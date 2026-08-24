set(engine_lib ${CMAKE_CURRENT_SOURCE_DIR})

add_library(sunken_castle SHARED
    "${engine_lib}/lib/glad/gl.c"
    "${engine_lib}/sunken_engine.cpp"
    "${engine_lib}/core/app.cpp"
    "${engine_lib}/renderer/mesh.cpp"
)

target_include_directories(sunken_castle PUBLIC 
    ${engine_lib}
    ${engine_lib}/lib
)

set_target_properties(sunken_castle PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(sunken_castle PRIVATE SANKARA_BUILD_SHARED)
target_link_libraries(sunken_castle PRIVATE glfw glm::glm opengl32 gdi32)