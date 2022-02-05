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

add_subdirectory(src)

set(EXAMPLE_FILE ${CMAKE_SOURCE_DIR}/example CACHE FILEPATH "Path to the pl0 file to execute when running compile_and_run_example (without the .pl0 extension)")
add_custom_target(compile_and_run_example
		BYPRODUCTS ${EXAMPLE_FILE}.bin
		BYPRODUCTS ${EXAMPLE_FILE}.asm
		BYPRODUCTS ${EXAMPLE_FILE}.json
		COMMAND pl0c ${EXAMPLE_FILE}
		COMMAND aassembler ${EXAMPLE_FILE}
		COMMAND aarest-console ${EXAMPLE_FILE}
		VERBATIM USES_TERMINAL)
add_dependencies(compile_and_run_example pl0c aassembler aarest-console)