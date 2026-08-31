use mlua::{IntoLua, Lua};
use std::ffi;
use std::io::Read;

use std::fs;
use std::sync::{LazyLock, Mutex};

use std::collections::HashMap;

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

static LUAU_VM: LazyLock<Lua> = LazyLock::new(|| Lua::new());

static CALLBACKS: LazyLock<Mutex<HashMap<i32, mlua::RegistryKey>>> = LazyLock::new(|| {
    let callback_hashmap = HashMap::new();
    Mutex::new(callback_hashmap)
});

extern "C" fn event_callback(callback: i32, event_id: i32, data: *mut ffi::c_void) {
    let map = CALLBACKS.lock().unwrap();

    if event_id as u32 == SK_EVENT_KEY_PRESS {
        let key = unsafe {
            let key_ptr = data as *const u8;
            *key_ptr
        };

        if map.contains_key(&callback) {
            let reg_key = map.get(&callback).unwrap();
            let event_fn = LUAU_VM
                .registry_value::<mlua::Function>(reg_key)
                .expect(format!("Could not find callback for event {}", callback).as_str());

            let mut args = mlua::MultiValue::new();
            args.push_back(key.into_lua(&LUAU_VM).unwrap());

            event_fn.call::<()>(args).expect("Event errored");
        }
    }
}

fn read_file(path: &str) -> String {
    let mut file = fs::File::open(path).unwrap();
    let mut contents = String::new();

    file.read_to_string(&mut contents).unwrap();
    contents
}

fn setup_luau() -> Result<(), Box<dyn std::error::Error>> {
    let callback_count: i32 = 0;

    let chunk = LUAU_VM.load(read_file("./demo_scripts/demo.luau"));

    let globals = LUAU_VM.globals();
    let camera_lib = LUAU_VM.create_table()?;

    let camera_move_fn = LUAU_VM.create_function(|_, (x, y, z): (f32, f32, f32)| {
        unsafe {
            skMoveView(x, y, z);
        }
        Ok(())
    })?;

    let camera_rot_fn = LUAU_VM.create_function(|_, (x, y, z): (f32, f32, f32)| {
        unsafe {
            skRotateView(x, y, z);
        }
        Ok(())
    })?;

    camera_lib.set("move", camera_move_fn)?;
    camera_lib.set("rotate", camera_rot_fn)?;

    globals.set("camera", camera_lib)?;

    let input_lib = LUAU_VM.create_table()?;

    let connect_key_press = LUAU_VM.create_function(move |_, arg_fn: mlua::Function| {
        let key = LUAU_VM.create_registry_value(arg_fn)?;
        let mut map = CALLBACKS.lock().unwrap();

        map.insert(callback_count, key);
        unsafe {
            let eventbinding = ffi::CString::new("KeyPress").unwrap();
            skListen(eventbinding.as_ptr(), callback_count);
        }

        Ok(())
    })?;

    input_lib.set("connectKeyPress", connect_key_press)?;

    globals.set("input", input_lib)?;

    chunk.exec()?;

    Ok(())
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    unsafe {
        if !skInit() {
            return Ok(());
        };
        let eventbinding = ffi::CString::new("KeyPress").unwrap();

        skEventCallback(Some(event_callback));
        skListen(eventbinding.into_raw(), 1);

        setup_luau()?;

        skRun();
    }

    Ok(())
}
