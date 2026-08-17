add_executable(Sandbox
	source/main.cpp
)

target_link_libraries(Sandbox
	PRIVATE
		Engine
		Editor
)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:Sandbox>/Resources
)