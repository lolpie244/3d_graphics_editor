add_executable(${PROJECT_NAME}
	main.cpp
	test_stage/test_stage.cpp
	test_stage1/test_stage1.cpp
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static)
target_include_directories(${PROJECT_NAME} PRIVATE .)
