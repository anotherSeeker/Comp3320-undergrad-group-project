use crate::bindings::bindings::{
    SK_ASSET, SK_ENTITY, skCreateObject, skMoveObject, skRotateObject,
};

pub struct UDObject {
    handle: SK_ENTITY,
    _mesh: SK_ASSET,
    _shader: SK_ASSET,
}

impl UDObject {
    pub fn new(mesh_handle: SK_ASSET, shader_handle: SK_ASSET) -> Self {
        Self {
            handle: unsafe { skCreateObject(mesh_handle, shader_handle) },
            _mesh: mesh_handle,
            _shader: shader_handle,
        }
    }
}

impl mlua::UserData for UDObject {
    fn add_methods<M: mlua::prelude::LuaUserDataMethods<Self>>(methods: &mut M) {
        methods.add_method("setPosition", |_, this, position: mlua::Vector| {
            unsafe {
                skMoveObject(this.handle, position.x(), position.y(), position.z());
            }
            Ok(())
        });

        methods.add_method("setOrientation", |_, this, orientation: mlua::Vector| {
            unsafe {
                skRotateObject(
                    this.handle,
                    orientation.x(),
                    orientation.y(),
                    orientation.z(),
                );
            }
            Ok(())
        });
    }
}
