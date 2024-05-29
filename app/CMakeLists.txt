add_executable(${PROJECT_NAME}
	main.cpp
	menu/menu.cpp

	editor/editor.cpp
	editor/editor_events.cpp
	editor/editor_gui.cpp
	editor/mode.cpp

	editor/network/collaborator.cpp
	editor/network/host.cpp
	editor/network/client.cpp
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static ${tinyfiledialogs})
target_include_directories(${PROJECT_NAME} PRIVATE .)
