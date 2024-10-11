# configure CMake tool
cmake_minimum_required(VERSION 3.5.0)

# configure project
project(XExamples VERSION 1.0.1 LANGUAGES CXX C)
add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(XExamples "source/example/XExamples.h" "source/example/main.cpp" )
add_subdirectory("source/kernel")
target_link_libraries(XExamples PUBLIC XElement::XElement )
