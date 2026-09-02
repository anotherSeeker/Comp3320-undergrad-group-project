use mlua::{IntoLua, Lua};
use std::ffi;
use std::io::Read;

use std::fs;
use std::sync::{LazyLock, Mutex};

use std::collections::HashMap;

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

static LUAU_VM: LazyLock<Lua> = LazyLock::new(|| Lua::new());
static CALLBACK_COUNT: LazyLock<Mutex<i32>> = LazyLock::new(|| Mutex::new(0));

static CALLBACKS: LazyLock<Mutex<HashMap<i32, mlua::RegistryKey>>> = LazyLock::new(|| {
    let callback_hashmap = HashMap::new();
    Mutex::new(callback_hashmap)
});

extern "C" fn event_callback(callback: i32, event_id: i32, data: *mut ffi::c_void) {
    let map = CALLBACKS.lock().unwrap();

    if !map.contains_key(&callback) {
        return;
    }

    let reg_key = map.get(&callback).unwrap();
    let event_fn = LUAU_VM
        .registry_value::<mlua::Function>(reg_key)
        .expect(format!("Could not find callback for event {}", callback).as_str());

    if event_id as u32 == SK_EVENT_KEY_PRESS || event_id as u32 == SK_EVENT_KEY_LIFTED {
        let key = unsafe {
            let key_ptr = data as *const u32;
            *key_ptr
        };

        let mut args = mlua::MultiValue::new();
        args.push_back(key.into_lua(&LUAU_VM).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    } else if event_id as u32 == SK_EVENT_PRERENDER {
        let dt = unsafe {
            let key_ptr = data as *const f64;
            *key_ptr
        };

        let reg_key = map.get(&callback).unwrap();
        let event_fn = LUAU_VM
            .registry_value::<mlua::Function>(reg_key)
            .expect(format!("Could not find callback for event {}", callback).as_str());

        let mut args = mlua::MultiValue::new();
        args.push_back(dt.into_lua(&LUAU_VM).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    } else if event_id as u32 == SK_EVENT_MOUSE_PRESS || event_id as u32 == SK_EVENT_MOUSE_LIFTED {
        let mousebutton = unsafe {
            let key_ptr = data as *const u8;
            *key_ptr
        };

        let reg_key = map.get(&callback).unwrap();
        let event_fn = LUAU_VM
            .registry_value::<mlua::Function>(reg_key)
            .expect(format!("Could not find callback for event {}", callback).as_str());

        let mut args = mlua::MultiValue::new();
        args.push_back(mousebutton.into_lua(&LUAU_VM).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    } else if event_id as u32 == SK_EVENT_MOUSE_MOVE {
        let mouse_data = unsafe {
            let key_ptr = data as *const SK_MOUSE_MOVE_EVENT;
            *key_ptr
        };

        let reg_key = map.get(&callback).unwrap();
        let event_fn = LUAU_VM
            .registry_value::<mlua::Function>(reg_key)
            .expect(format!("Could not find callback for event {}", callback).as_str());

        let mut args = mlua::MultiValue::new();
        args.push_back(mouse_data.mouseX.into_lua(&LUAU_VM).unwrap());
        args.push_back(mouse_data.mouseY.into_lua(&LUAU_VM).unwrap());
        args.push_back(mouse_data.deltaX.into_lua(&LUAU_VM).unwrap());
        args.push_back(mouse_data.deltaY.into_lua(&LUAU_VM).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    }
}

fn read_file(path: &str) -> String {
    let mut file = fs::File::open(path).unwrap();
    let mut contents = String::new();

    file.read_to_string(&mut contents).unwrap();
    contents
}

fn register_callback(arg_fn: mlua::Function) -> Result<(), Box<dyn std::error::Error>> {
    let key = LUAU_VM.create_registry_value(arg_fn)?;
    let mut map = CALLBACKS.lock().unwrap();

    let callback_count = CALLBACK_COUNT.lock().unwrap();

    map.insert(*callback_count, key);

    Ok(())
}

fn create_event_fn(event_name: &str) -> Result<mlua::Function, Box<dyn std::error::Error>> {
    let eventbinding = ffi::CString::new(event_name).unwrap();

    let event_fn = LUAU_VM.create_function(move |_, arg_fn: mlua::Function| {
        register_callback(arg_fn).unwrap();
        let mut callback_count = CALLBACK_COUNT.lock().unwrap();

        unsafe {
            skListen(eventbinding.as_ptr(), *callback_count);
        }

        *callback_count += 1;

        Ok(())
    })?;

    Ok(event_fn)
}

fn setup_luau() -> Result<(), Box<dyn std::error::Error>> {
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

    input_lib.set("connectKeyPress", create_event_fn("KeyPress")?)?;
    input_lib.set("connectKeyLifted", create_event_fn("KeyLifted")?)?;

    input_lib.set("connectMousePress", create_event_fn("MousePress")?)?;
    input_lib.set("connectMouseLifted", create_event_fn("MouseLifted")?)?;
    input_lib.set("connectMouseMove", create_event_fn("MouseMove")?)?;

    globals.set("input", input_lib)?;

    let runservice_lib = LUAU_VM.create_table()?;
    runservice_lib.set("connectPreRender", create_event_fn("PreRender")?)?;

    globals.set("runservice", runservice_lib)?;

    chunk.exec()?;

    Ok(())
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    unsafe {
        if !skInit() {
            return Ok(());
        };

        skEventCallback(Some(event_callback));

        setup_luau()?;

        skRun();
    }

    Ok(())
}
