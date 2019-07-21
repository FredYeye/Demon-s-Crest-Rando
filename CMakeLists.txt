cmake_minimum_required(VERSION 2.8.3)
set(project_name DCRando)
project(${project_name})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -static")

set(source_files
	src/main.cpp
	src/random.cpp
	src/file.cpp
	)

set(header_files
	src/main.hpp
	src/random.hpp
	src/file.hpp
	)

add_executable(${project_name} ${header_files} ${source_files})
