add_subdirectory(aassembler)

cmake_policy(SET CMP0098 OLD)
find_package(FLEX)
FLEX_TARGET(lexer lexer.lex lexer.cpp DEFINES_FILE lexer.hpp)

add_executable(pl0 ${FLEX_lexer_OUTPUTS} main.cpp parser.cpp token_type.cpp context.cpp logger.cpp stmt_list.cpp runtime.cpp stack.cpp optimization.cpp)
target_link_libraries(pl0 PUBLIC ${FLEX_LIBRARIES})

add_executable(pl0c ${FLEX_lexer_OUTPUTS} main.cpp parser.cpp token_type.cpp context.cpp logger.hpp logger.cpp stmt_list.cpp code_gen.cpp stack.cpp stack.hpp optimization.cpp vm_emitter.cpp)
target_link_libraries(pl0c PUBLIC ${FLEX_LIBRARIES})
target_compile_definitions(pl0c PRIVATE BUILD_COMPILER)
add_dependencies(pl0c aassembler aarest-console)