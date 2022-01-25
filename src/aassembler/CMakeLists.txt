cmake_policy(SET CMP0098 OLD)
find_package(FLEX)
FLEX_TARGET(aassembler-lexer aassembler.l aassembler.cpp DEFINES_FILE aassembler.hpp)

add_executable(aassembler ${FLEX_aassembler-lexer_OUTPUTS})
target_link_libraries(aassembler PUBLIC ${FLEX_LIBRARIES})

add_executable(aarest-console aarest-console.cpp stack.cpp)