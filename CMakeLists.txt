cmake_minimum_required(VERSION 3.10.0)
project(sunken_castle VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(sandbox_dir "${CMAKE_SOURCE_DIR}/sandbox")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sandbox_dir})

add_subdirectory(sandbox) # file for testing, to compile create a folder called 'sandbox' with a CMakeLists.txt inside it
add_subdirectory(engine)

include(FetchContent)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.5.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.3.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(glfw glm)
