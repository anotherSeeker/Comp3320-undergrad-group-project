# the sunken castle (Comp3320-undergrad-group-project)

## languages
- C++
- rust
- [luau](https://luau.org/)

## libraries

- C++ libraries
  - openGL (atleast version 4.3 but 4.6 is reccomended)
  - GLFW
  - GLAD2
  - GLM
- rust crates
  - bindgen
  - mlua

## tooling
- gnu compiler collection (MINGW64)
- cmake
- cargo

## style guide
if using rust then stick to rust's style guide else follow this.

- SCREAMING_SNAKE_CASE for constants and macros.
- camelCase for methods, functions, variables and class properties.
- PascalCase for everything else (classes).

## practices

- Avoid exceptions
    - Exceptions cause memory overhead and introduce hidden control flow.  
      Instead op for returning booleans and using std::cerr.
    - This doesn't apply for non critical tasks like asset loading.

- Build types
  - There are 3 cmake presets defined for compiling the engine:
    - debug: for debugging the engine.
    - dev: for fast prototyping and iteration.
    - release: for production.

  - there are 2 variants for each of these presets:
    - runtime for deploying the .dll to the runtime folder
    - sandbox for building your sandbox C++ code and linking it with the library.
