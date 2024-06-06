add_executable(${PROJECT_NAME}-server
	server.cpp
)

target_include_directories(${PROJECT_NAME}-server PRIVATE . ${CMAKE_BINARY_DIR}/include)

