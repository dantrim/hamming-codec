cmake_minimum_required(VERSION 3.15...3.18)
project(hamming-codec VERSION "0.0.1")

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
#find_package(pybind11 CONFIG REQUIRED)
#add_subdirectory(cpp)

## Currently, Scikit-build does not support FindPython, so we convert the
## provided hints ourselves.
#if(SKBUILD)
#  set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
#  set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
#  set(Python_LIBRARY "${PYTHON_LIBRARY}")
#  set(DUMMY "${PYTHON_VERSION_STRING}")  # Not needed, silences a warning
#endif()
#
#set(Python_FIND_IMPLEMENTATIONS CPython PyPy)  # PyPy requires 3.18
#find_package(Python REQUIRED COMPONENTS Interpreter Development)
#
## Scikit-Build does not add your site-packages to the search path automatically,
## so we need to add it _or_ the pybind11 specific directory here.
#execute_process(
#  COMMAND
#    "${Python_EXECUTABLE}" -c
#    "import pybind11; print(pybind11.get_cmake_dir())"
#  OUTPUT_VARIABLE _tmp_dir
#  OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
#message(STATUS "_tmp_dir = ${_tmp_dir}")
#list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
#
## Now we can find pybind11
#find_package(pybind11 CONFIG REQUIRED)
#
#pybind11_add_module(_hamming_codec MODULE cpp/python.cpp)
#
#target_compile_definitions(_hamming_codec PRIVATE VERSION_INFO=${PROJECT_VERSION})
#
#install(TARGETS _hamming_codec DESTINATION ${CMAKE_BINARY_DIR}/lib)
