use mlua::IntoLua;
use std::ffi;
use std::path::PathBuf;
use std::sync::{LazyLock, Mutex};

mod runtime;
mod ud_object;

static RUNTIME: LazyLock<Mutex<runtime::Runtime>> =
    LazyLock::new(|| Mutex::new(runtime::Runtime::new()));

mod bindings;
use bindings::bindings::{
    SK_EVENT_KEY_LIFTED, SK_EVENT_KEY_PRESS, SK_EVENT_MOUSE_LIFTED, SK_EVENT_MOUSE_MOVE,
    SK_EVENT_MOUSE_PRESS, SK_EVENT_PRERENDER, SK_MOUSE_MOVE_EVENT, skEventCallback, skInit, skRun,
};

extern "C" fn event_callback(callback: i32, event_id: i32, data: *mut ffi::c_void) {
    let runtime = RUNTIME.lock().unwrap();
    let state = runtime.state.lock().unwrap();
    let callbacks = &state.callbacks;

    if !callbacks.contains_key(&callback) {
        return;
    }

    let reg_key = callbacks.get(&callback).unwrap();
    let event_fn = runtime
        .vm
        .registry_value::<mlua::Function>(reg_key)
        .expect(format!("Could not find callback for event {}", callback).as_str());

    if event_id as u32 == SK_EVENT_KEY_PRESS || event_id as u32 == SK_EVENT_KEY_LIFTED {
        let key = unsafe {
            let key_ptr = data as *const u32;
            *key_ptr
        };

        let mut args = mlua::MultiValue::new();
        args.push_back(key.into_lua(&runtime.vm).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    } else if event_id as u32 == SK_EVENT_PRERENDER {
        let dt = unsafe {
            let key_ptr = data as *const f64;
            *key_ptr
        };

        let reg_key = callbacks.get(&callback).unwrap();
        let event_fn = runtime
            .vm
            .registry_value::<mlua::Function>(reg_key)
            .expect(format!("Could not find callback for event {}", callback).as_str());

        let mut args = mlua::MultiValue::new();
        args.push_back(dt.into_lua(&runtime.vm).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    } else if event_id as u32 == SK_EVENT_MOUSE_PRESS || event_id as u32 == SK_EVENT_MOUSE_LIFTED {
        let mousebutton = unsafe {
            let key_ptr = data as *const u8;
            *key_ptr
        };

        let reg_key = callbacks.get(&callback).unwrap();
        let event_fn = runtime
            .vm
            .registry_value::<mlua::Function>(reg_key)
            .expect(format!("Could not find callback for event {}", callback).as_str());

        let mut args = mlua::MultiValue::new();
        args.push_back(mousebutton.into_lua(&runtime.vm).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    } else if event_id as u32 == SK_EVENT_MOUSE_MOVE {
        let mouse_data = unsafe {
            let key_ptr = data as *const SK_MOUSE_MOVE_EVENT;
            *key_ptr
        };

        let reg_key = callbacks.get(&callback).unwrap();
        let event_fn = runtime
            .vm
            .registry_value::<mlua::Function>(reg_key)
            .expect(format!("Could not find callback for event {}", callback).as_str());

        let mut args = mlua::MultiValue::new();
        args.push_back(mouse_data.mouseX.into_lua(&runtime.vm).unwrap());
        args.push_back(mouse_data.mouseY.into_lua(&runtime.vm).unwrap());
        args.push_back(mouse_data.deltaX.into_lua(&runtime.vm).unwrap());
        args.push_back(mouse_data.deltaY.into_lua(&runtime.vm).unwrap());

        event_fn.call::<()>(args).expect("Event errored");
    }
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    unsafe {
        if !skInit() {
            return Ok(());
        };

        skEventCallback(Some(event_callback));
        let mut runtime = RUNTIME.lock().unwrap();
        runtime.run(PathBuf::from("./demo_scripts/demo.luau"));

        std::mem::drop(runtime);
        skRun();
    }

    Ok(())
}
