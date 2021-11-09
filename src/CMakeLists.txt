cmake_policy(SET CMP0098 OLD)
find_package(FLEX)
FLEX_TARGET(lexer lexer.lex lexer.c DEFINES_FILE lexer.h)

add_executable(main main.c ${FLEX_lexer_OUTPUTS})
target_link_libraries(main ${FLEX_LIBRARIES})