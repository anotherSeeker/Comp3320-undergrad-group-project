use std::ffi;
use std::fs;
use std::io::Read;
use std::sync::{Arc, Mutex};
use std::{collections::HashMap, path::PathBuf};

use crate::bindings::bindings::{skListen, skLoadMesh, skLoadShader, skMoveView, skRotateView};
use crate::ud_object;
pub struct RuntimeState {
    pub callback_count: i32,
    pub callbacks: HashMap<i32, mlua::RegistryKey>,
}

pub struct Runtime {
    pub vm: mlua::Lua,
    pub state: Arc<Mutex<RuntimeState>>,
}

fn read_file(path: &PathBuf) -> String {
    let mut file = fs::File::open(path).unwrap();
    let mut contents = String::new();

    file.read_to_string(&mut contents).unwrap();
    contents
}

impl Runtime {
    pub fn new() -> Self {
        let mut runtime = Self {
            vm: mlua::Lua::new(),
            state: Arc::new(Mutex::new(RuntimeState {
                callback_count: 0,
                callbacks: HashMap::new(),
            })),
        };

        match runtime.setup_libs() {
            Ok(_) => println!("sucessfully loaded libraries"),
            Err(err) => println!("{}", format!("failed to load libraries\n{}", err)),
        };

        runtime
    }

    fn create_event_fn(
        &mut self,
        event_name: &str,
    ) -> Result<mlua::Function, Box<dyn std::error::Error>> {
        let eventbinding = ffi::CString::new(event_name).unwrap();

        let state = Arc::clone(&self.state);

        let event_fn =
            self.vm
                .create_function(move |context: &mlua::Lua, arg_fn: mlua::Function| {
                    let mut state = state.lock().unwrap();
                    let key = context.create_registry_value(arg_fn)?;
                    let callback_id = state.callback_count;

                    state.callbacks.insert(callback_id, key);

                    unsafe {
                        skListen(eventbinding.as_ptr(), state.callback_count);
                    }

                    state.callback_count += 1;

                    Ok(())
                })?;

        Ok(event_fn)
    }

    fn get_enum_table(&self) -> Result<mlua::Table, mlua::error::Error> {
        let chunk = self.vm.load(read_file(&PathBuf::from("./enum.luau")));

        let value = chunk.eval::<mlua::Table>()?;

        Ok(value)
    }

    fn setup_libs(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        let globals = self.vm.globals();
        let camera_lib = self.vm.create_table()?;

        let camera_move_fn = self.vm.create_function(|_, (x, y, z): (f32, f32, f32)| {
            unsafe {
                skMoveView(x, y, z);
            }
            Ok(())
        })?;

        let camera_rot_fn = self.vm.create_function(|_, (x, y, z): (f32, f32, f32)| {
            unsafe {
                skRotateView(x, y, z);
            }
            Ok(())
        })?;

        camera_lib.set("move", camera_move_fn)?;
        camera_lib.set("rotate", camera_rot_fn)?;

        globals.set("camera", camera_lib)?;

        let input_lib = self.vm.create_table()?;

        input_lib.set("connectKeyPress", self.create_event_fn("KeyPress")?)?;
        input_lib.set("connectKeyLifted", self.create_event_fn("KeyLifted")?)?;

        input_lib.set("connectMousePress", self.create_event_fn("MousePress")?)?;
        input_lib.set("connectMouseLifted", self.create_event_fn("MouseLifted")?)?;
        input_lib.set("connectMouseMove", self.create_event_fn("MouseMove")?)?;

        globals.set("input", input_lib)?;

        let runservice_lib = self.vm.create_table()?;
        runservice_lib.set("connectPreRender", self.create_event_fn("PreRender")?)?;

        globals.set("runservice", runservice_lib)?;

        let object_fn = self.vm.create_function(
            |_,
             (mesh_luapath, vertex_luapath, fragment_luapath): (
                mlua::LuaString,
                mlua::LuaString,
                mlua::LuaString,
            )| {
                let mesh_path = std::ffi::CString::new(mesh_luapath.to_string_lossy()).unwrap();
                let vertex_path = std::ffi::CString::new(vertex_luapath.to_string_lossy()).unwrap();
                let fragment_path =
                    std::ffi::CString::new(fragment_luapath.to_string_lossy()).unwrap();

                unsafe {
                    let mesh_handle = skLoadMesh(mesh_path.as_ptr());
                    let shader_handle = skLoadShader(vertex_path.as_ptr(), fragment_path.as_ptr());

                    Ok(ud_object::UDObject::new(mesh_handle, shader_handle))
                }
            },
        )?;

        globals.set("createObject", object_fn)?;
        globals.set("enum", self.get_enum_table()?)?;
        Ok(())
    }

    pub fn run(&mut self, path: &PathBuf) {
        let chunk = self.vm.load(read_file(path));

        match chunk.exec() {
            Ok(_) => println!("Successfully ran script"),
            Err(err) => eprintln!("{}", format!("Failed to run script\n{}", err)),
        }
    }
}
