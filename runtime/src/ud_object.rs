use crate::bindings::bindings::{SK_ENTITY, skMoveObject, skRotateObject};

struct UDObject {
    handle: SK_ENTITY,
}

impl mlua::UserData for UDObject {
    fn add_methods<M: mlua::prelude::LuaUserDataMethods<Self>>(methods: &mut M) {
        methods.add_method("move", |_, this, (x, y, z): (f32, f32, f32)| {
            unsafe {
                skMoveObject(this.handle, x, y, z);
            }
            Ok(())
        });

        methods.add_method("rotate", |_, this, (x, y, z): (f32, f32, f32)| {
            unsafe {
                skRotateObject(this.handle, x, y, z);
            }
            Ok(())
        });
    }
}
