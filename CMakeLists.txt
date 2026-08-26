cmake_minimum_required(VERSION 3.10.0)
project(sunken_castle VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(sandbox_dir "${CMAKE_SOURCE_DIR}/sandbox")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sandbox_dir})

find_program(CCACHE ccache)

if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    message(STATUS "ccache found")
else()
    message(STATUS "ccache not found")
endif()


add_subdirectory(sandbox) # file for testing, to compile create a folder called 'sandbox' with a CMakeLists.txt inside it
add_subdirectory(engine)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(glfw glm)
