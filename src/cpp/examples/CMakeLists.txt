##
## create executables
##
file(GLOB execfiles [a-zA-Z]*.cpp)
foreach(target ${execfiles})
    get_filename_component(execname ${target} NAME_WE)
    get_filename_component(srcfile ${target} NAME)
    message(STATUS "Adding: executable ${execname}")
    add_executable(${execname})
    target_sources(${execname} PRIVATE ${srcfile})
    target_include_directories(${execname} PRIVATE ${HAMMING_CODEC_INCLUDE_DIRS})
endforeach()
