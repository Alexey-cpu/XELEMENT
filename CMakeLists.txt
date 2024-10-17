# configure CMake tool
cmake_minimum_required(VERSION 3.5.0)

# configure project
project(XExamples VERSION 1.0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_definitions("/wd4251")
  add_definitions("/wd4244")
  add_definitions("/wd4267")
  add_definitions("/wd4996")
  add_definitions("/wd4723")
  add_definitions("/wd4101")
  add_definitions("/wd4018")
  add_definitions("/wd4068")
  add_definitions("/wd4834")
  add_definitions("/wd4334")
  add_compile_options(/Ox)
else()

    # This works only for Release build
    add_compile_options(-O3)

endif(WIN32)

add_executable(XExamples "source/example/XExamples.h" "source/example/main.cpp" )
add_subdirectory("source/kernel")
target_link_libraries(XExamples PUBLIC XElement::XElement )
