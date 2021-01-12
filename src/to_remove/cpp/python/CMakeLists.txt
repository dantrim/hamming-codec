if(SKBUILD)
  set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
  set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
  set(Python_LIBRARY "${PYTHON_LIBRARY}")
  set(DUMMY "${PYTHON_VERSION_STRING}")  # Not needed, silences a warning
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
target_compile_definitions(_hamming_codec PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_link_libraries(_hamming_codec PRIVATE hamming_codec)
install(TARGETS _hamming_codec DESTINATION .)
