use std::ffi;
use std::os::raw::c_char;

type EventCallbackFn = extern "C" fn(i32, i32, *mut ffi::c_void);

#[link(name = "sunken_castle.dll", kind = "dylib")]
unsafe extern "C" {
    pub fn skInit() -> bool;
    pub fn skRun();

    pub fn skEventCallback(eventCallback: EventCallbackFn);
    pub fn skListen(eventName: *const c_char, callback: i32);

    pub fn skLog(message: *const c_char);

    pub fn skMoveView(x: f32, y: f32, z: f32);
    pub fn skRotateView(x: f32, y: f32, z: f32);
}

fn main() {
    unsafe {
        if !skInit() {
            return;
        };
        let message = ffi::CString::new("hello world").expect("failed to create c string");

        skLog(message.as_ptr());
        skRun();
    }
}
