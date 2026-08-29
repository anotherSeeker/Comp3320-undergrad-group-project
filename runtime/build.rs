use std::env;
use std::path::PathBuf;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let bindings = bindgen::Builder::default()
        .header("../engine/sunken_engine.hpp")
        .generate()?;

    let out_path = PathBuf::from(env::var("OUT_DIR")?);

    println!("{}", out_path.display());
    bindings.write_to_file(out_path.join("bindings.rs"))?;

    let project_dir = env::var("CARGO_MANIFEST_DIR").unwrap();

    println!("cargo:rustc-link-search=native={}", project_dir);
    println!("cargo:rustc-link-lib=dylib=sunken_castle");

    Ok(())
}
