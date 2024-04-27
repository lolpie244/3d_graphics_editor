add_executable(${PROJECT_NAME}
	main.cpp
	menu/menu.cpp

	editor/editor.cpp
	editor/gizmo.cpp
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static)
target_include_directories(${PROJECT_NAME} PRIVATE .)
