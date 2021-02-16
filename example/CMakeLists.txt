project("example")


add_executable(example "src/main.cpp" "src/button/button.h" )

target_link_libraries(example oxide)

add_custom_command(TARGET example POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_CURRENT_LIST_DIR}/resources $<TARGET_FILE_DIR:example>/resources)