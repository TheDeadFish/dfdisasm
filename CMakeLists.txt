project(dfdisasm)
cmake_minimum_required(VERSION 3.16)

set( EXECUTABLE_OUTPUT_PATH ../bin)
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)

add_executable(test src/basicBlock.cc src/disa.cc src/disa_core.cc 
	src/dfdisasm.cc src/test.cc)
target_link_libraries(test pdbLib -lpeFile -lstdshit -ludis86)
