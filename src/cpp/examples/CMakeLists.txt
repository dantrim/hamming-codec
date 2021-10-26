#
# encode
#
message(STATUS "Adding executable: example_encode")
add_executable(example_encode)
target_sources(example_encode PRIVATE example_encode.cpp)
target_include_directories(example_encode PRIVATE ${HAMMING_CODEC_INCLUDE_DIRS})

#
# decode
#
message(STATUS "Adding executable: example_decode")
add_executable(example_decode)
target_sources(example_decode PRIVATE example_decode.cpp)
target_include_directories(example_decode PRIVATE ${HAMMING_CODEC_INCLUDE_DIRS})
