use std::ffi;

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

extern "C" fn event_callback(callback: i32, event_id: i32, data: *mut ffi::c_void) {
    let key = unsafe {
        let key_ptr = data as *const u8 as *const char;
        *key_ptr
    };

    println!("{} {} {}", callback, event_id, key)
}

fn main() {
    unsafe {
        if !skInit() {
            return;
        };
        let eventbinding = ffi::CString::new("KeyPress").unwrap();

        skEventCallback(Some(event_callback));

        skListen(eventbinding.into_raw(), 1);
        skRun();
    }
}
