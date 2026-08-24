set(engine_lib ${CMAKE_CURRENT_SOURCE_DIR})

add_library(sunken_castle SHARED 
    "${engine_lib}/sunken_engine.cpp"
)

target_include_directories(sunken_castle PUBLIC 
    ${engine_lib}
)

set_target_properties(sunken_castle PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(sunken_castle PRIVATE SANKARA_BUILD_SHARED)