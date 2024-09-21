# configure CMake tool
cmake_minimum_required(VERSION 3.5.0)

# configure project
project(XElement VERSION 1.0.1 LANGUAGES CXX C)
add_compile_options(-Wall -Wextra -Wpedantic -O3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_library(XElement "XElement.h" "XElement.cpp" )
add_library(XElement::XElement ALIAS XElement)
target_include_directories(XElement PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
