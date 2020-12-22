cmake_minimum_required(VERSION 3.12)
project(hamming-codec)

##
## setup the build output structure
##
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
# if we are in a virtual environment, take that python version
# and require that there are no ambiguities
set(HAMMING_CODEC_PYTHON_PATH "")
if (NOT $ENV{VIRTUAL_ENV} STREQUAL "")
    execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --green --bold "-- Virtual environment detected (VIRTUAL_ENV=$ENV{VIRTUAL_ENV})")
    set(lib_match_string "$ENV{VIRTUAL_ENV}/lib/python3.*")#python*/site-packages")
    file(GLOB matching_dirs LIST_DIRECTORIES true ${lib_match_string})
    set(matching_dirs_list ${matching_dirs})
    separate_arguments(matching_dirs_list)
    list(LENGTH matching_dirs_list n_libs_found)
    if(NOT ${n_libs_found} STREQUAL "1")
        execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --red --bold "-- More than one python library found in virtual-environment:")
        foreach(lib_found ${matching_dirs_list})
            execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --red --bold "-- > ${lib_found}")
        endforeach()
        message(FATAL_ERROR "Cannot continue due to ambiguous python environment")
    endif()
    set(python_path "${matching_dirs}/site-packages")
    set(HAMMING_CODEC_PYTHON_PATH ${python_path})
elseif (NOT $ENV{PYTHONPATH} STREQUAL "")
    execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --green --bold "-- Picking up python library from defined environment PYTHONPATH=$ENV{PYTHONPATH}")
    set(HAMMING_CODEC_PYTHON_PATH $ENV{PYTHONPATH})
endif()

if( "${HAMMING_CODEC_PYTHON_PATH}" STREQUAL "" )
    execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --red --bold "-- Could not determine proper python library to use")
    execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --red --bold "-- Either run in a virtual environment or set PYTHONPATH properly (e.g. to where hamming_codec module is installed, /foo/bar/lib/python3.X/site-packages)")
    message(FATAL_ERROR "Cannot continue due to ambiguous python environment")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --green --bold "-- Will use hamming_codec module from python libary site-packages: ${HAMMING_CODEC_PYTHON_PATH}")

add_subdirectory(cpp)
add_subdirectory(exts)
include_directories(exts/pybind11/include)
