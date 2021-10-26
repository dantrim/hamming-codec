if(SKBUILD)
  message(STATUS "FOO SKBUILD")
  set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
  set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
  set(Python_LIBRARY "${PYTHON_LIBRARY}")
  set(Python_LIBRARIES "${PYTHON_LIBRARIES}")
  set(DUMMY "${PYTHON_VERSION_STRING}")  # Not needed, silences a warning
  message(STATUS "Python_EXECUTABLE = ${Python_EXECUTABLE}")
  message(STATUS "Python_INCLUDE_DIR = ${Python_INCLUDE_DIR}")
  message(STATUS "Python_LIBRARY = ${Python_LIBRARY}")
  message(STATUS "Python_LIBRARIES = ${Python_LIBRARIES}")
endif()
set(Python_FIND_IMPLEMENTATIONS CPython)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
execute_process(
  COMMAND
    "${Python_EXECUTABLE}" -c
    "import pybind11; print(pybind11.get_cmake_dir())"
  OUTPUT_VARIABLE _tmp_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
message(STATUS "_tmp_dir = ${_tmp_dir}")
list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
pybind11_add_module(_hamming_codec MODULE module.cpp)
target_compile_definitions(_hamming_codec PRIVATE VERSION_INFO="${PROJECT_VERSION}")
target_include_directories(_hamming_codec PRIVATE ${HAMMING_CODEC_INCLUDE_DIRS})
install(TARGETS _hamming_codec DESTINATION .)
