cmake_minimum_required(VERSION 3.15)
project(hamming-codec VERSION "0.1.0")

##
## setup the build output structure
##
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_PYTHON "Build the python bindings" OFF)
add_subdirectory(exts/pybind11 EXCLUDE_FROM_ALL)
add_subdirectory(src)
