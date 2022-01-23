cmake_policy(SET CMP0098 OLD)
find_package(FLEX)
FLEX_TARGET(lexer lexer.lex lexer.cpp DEFINES_FILE lexer.hpp)

add_executable(pl-0 ${FLEX_lexer_OUTPUTS} main.cpp parser.cpp token_type.cpp context.cpp logger.hpp logger.cpp stmt_list.hpp stmt_list.cpp runtime.hpp runtime.cpp stack.cpp stack.hpp optimization.hpp optimization.cpp)
target_link_libraries(pl-0 PUBLIC ${FLEX_LIBRARIES})