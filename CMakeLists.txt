cmake_minimum_required(VERSION 3.15)
project(hamming-codec VERSION "0.2.4")

##
## setup the build output structure
##
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.3)
        message(FATAL_ERROR "GCC version must be at least 7.3")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        message(FATAL_ERROR "Clang version must be at least 6.0")
    endif()
else()
    message(WARNING "Untested compiler \"${CMAKE_CXX_COMPILER_ID}\", proceed with caution!")
endif()

option(BUILD_PYTHON "Build the python bindings" OFF)
add_subdirectory(exts/pybind11 EXCLUDE_FROM_ALL)
add_subdirectory(src)
