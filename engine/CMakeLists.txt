set(engine_lib ${CMAKE_CURRENT_SOURCE_DIR})

add_library(sunken_castle SHARED
    "${engine_lib}/lib/glad/gl.c"
    "${engine_lib}/sunken_engine.cpp"

    "${engine_lib}/core/app.cpp"
    "${engine_lib}/core/debugger.cpp"
    "${engine_lib}/core/scene_graph.cpp"

    "${engine_lib}/renderer/mesh.cpp"
    "${engine_lib}/renderer/shader.cpp"
    "${engine_lib}/renderer/renderer.cpp"

    "${engine_lib}/core/events/events.cpp"
    
)

target_include_directories(sunken_castle PUBLIC 
    ${engine_lib}
    ${engine_lib}/lib
)

set_target_properties(sunken_castle PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(sunken_castle PRIVATE
    SK_DEBUG=0
    SK_DEV=1
    SK_RELEASE=2

    $<$<CONFIG:Debug>:SK_BUILD=0>
    $<$<CONFIG:RelWithDebInfo>:SK_BUILD=1>
    $<$<CONFIG:Release>:SK_BUILD=2>
)

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")

target_link_libraries(sunken_castle PRIVATE glfw glm::glm EnTT::EnTT opengl32 gdi32)