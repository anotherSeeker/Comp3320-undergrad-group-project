# Comp3320-undergrad-group-project

## libraries
- openGL
- GLFW
- GLAD2
- GLM

## tooling
- gnu compiler collection (MINGW64)
- cmake

## style guide

- SCREAMING_SNAKE_CASE for constants and macros
- camelCase for methods, functions, variables and class properties
- PascalCase for everything else (classes)

## practices

- Avoid exceptions
    - Exceptions cause memory overhead and introduce hidden control flow.  
      Instead op for returning booleans and using std::cerr
