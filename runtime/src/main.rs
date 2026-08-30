use mlua::Lua;
use std::ffi;
use std::io::Read;

use std::fs;

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

extern "C" fn event_callback(callback: i32, event_id: i32, data: *mut ffi::c_void) {
    if event_id as u32 == SK_EVENT_SK_EVENT_KEY_PRESS {
        let key = unsafe {
            let key_ptr = data as *const u8 as *const char;
            *key_ptr
        };

        println!("{} {} {}", callback, event_id, key)
    }
}

fn read_file(path: &str) -> String {
    let mut file = fs::File::open(path).unwrap();
    let mut contents = String::new();

    file.read_to_string(&mut contents).unwrap();
    contents
}

fn setup_luau(luau_vm: &Lua) -> Result<(), Box<dyn std::error::Error>> {
    let chunk = luau_vm.load(read_file("./demo_scripts/demo.luau"));

    let globals = luau_vm.globals();
    let camera_lib = luau_vm.create_table()?;

    let camera_move_fn = luau_vm.create_function(|_, (x, y, z): (f32, f32, f32)| {
        unsafe {
            skMoveView(x, y, z);
        }
        Ok(())
    })?;

    let camera_rot_fn = luau_vm.create_function(|_, (x, y, z): (f32, f32, f32)| {
        unsafe {
            skRotateView(x, y, z);
        }
        Ok(())
    })?;

    camera_lib.set("move", camera_move_fn)?;
    camera_lib.set("rotate", camera_rot_fn)?;

    globals.set("camera", camera_lib)?;

    let input_lib = luau_vm.create_table()?;

    let connect_key_press = luau_vm.create_function(|_, arg_fn: mlua::Function| {
        arg_fn.call::<()>(())?;

        Ok(())
    })?;

    input_lib.set("connectKeyPress", connect_key_press)?;

    globals.set("input", input_lib)?;

    chunk.exec()?;

    Ok(())
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let luau_vm = &Lua::new();

    unsafe {
        if !skInit() {
            return Ok(());
        };
        let eventbinding = ffi::CString::new("KeyPress").unwrap();

        skEventCallback(Some(event_callback));
        skListen(eventbinding.into_raw(), 1);

        setup_luau(luau_vm)?;

        skRun();
    }

    Ok(())
}
