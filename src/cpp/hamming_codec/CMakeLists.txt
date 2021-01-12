message(STATUS "Creating C++ embedding of hamming_codec module")
if (BUILD_PYTHON)
    add_library(hamming_codec STATIC)
else()
    add_library(hamming_codec SHARED)
endif()
target_sources(hamming_codec
    PRIVATE
    hamming_codec.cpp
    utils.cpp
)
set_target_properties(hamming_codec PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(hamming_codec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
configure_file( utils.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp @ONLY )
