add_executable(sandbox main.cpp)

target_link_libraries(sandbox PRIVATE sunken_castle)
set_target_properties(sandbox PROPERTIES OUTPUT_NAME "game")
