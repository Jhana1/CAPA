cmake_minimum_required(VERSION 3.2)
message("CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES: ${CMAKE_PLATFORM_IMPLICIT_INCLUDE_DIRECTORIES}"})
# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable (gcc-loops gcc-loops.cpp)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello


