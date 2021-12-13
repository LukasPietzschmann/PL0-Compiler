cmake_policy(SET CMP0098 OLD)
find_package(FLEX)
FLEX_TARGET(lexer lexer.lex lexer.cpp DEFINES_FILE lexer.hpp)

add_executable(main ${FLEX_lexer_OUTPUTS} main.cpp parser.cpp token_type.cpp)
target_link_libraries(main PUBLIC ${FLEX_LIBRARIES})