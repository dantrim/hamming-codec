message(STATUS "Creating C++ embedding of hamming_codec module")
add_library("hamming_codec_cpp" SHARED)
target_sources(hamming_codec_cpp
    PRIVATE
    hamming_codec.cpp
)
target_link_libraries(hamming_codec_cpp PRIVATE pybind11::embed)
target_include_directories(hamming_codec_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
