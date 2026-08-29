cmake_minimum_required(VERSION 3.10.0)
project(sunken_castle VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(runtime_dir "${CMAKE_SOURCE_DIR}/runtime")
set(sandbox_dir "${CMAKE_SOURCE_DIR}/sandbox")
option(BUILD_SANDBOX "builds the sandbox folder" OFF)

if(BUILD_SANDBOX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${sandbox_dir})
add_subdirectory(sandbox) # file for testing, to compile create a folder called 'sandbox' with a CMakeLists.txt inside it
else()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${runtime_dir})
endif()

add_subdirectory(engine)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/engine/lib/vendor")

include(FetchContent)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.5.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_DX11 OFF CACHE BOOL "")

FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.3.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(glfw glm)
