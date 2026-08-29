use mlua::Lua;
use std::ffi;

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

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let luau_vm = &Lua::new();

    let chunk = luau_vm.load(
        r#"
        const message : string = "hello world"
    print(message)
    "#,
    );

    chunk.exec()?;

    unsafe {
        if !skInit() {
            return Ok(());
        };
        let eventbinding = ffi::CString::new("KeyPress").unwrap();

        skEventCallback(Some(event_callback));

        skListen(eventbinding.into_raw(), 1);
        skRun();
    }

    Ok(())
}
