cmake_minimum_required(VERSION 3.19)
project("PL/0 Compiler" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

option(COMPILE_FOR_AAREST_WEB "Set this option, to stop generate dec opcodes, as the web interface does not support this" OFF)
if (${COMPILE_FOR_AAREST_WEB})
	add_compile_definitions(COMPILE_FOR_AAREST_WEB)
endif ()

option(COMPILE_INTERPRETER "Set this option to compile the interpreter instead of the code generation" OFF)
if (${COMPILE_INTERPRETER})
	add_compile_definitions(COMPILE_INTERPRETER)
endif ()

add_subdirectory(src)