#pragma once

#include <initializer_list>
#include <optional>
#include <string>

#include "context.hpp"
#include "stmt_list.hpp"
#include "optimization.hpp"
#include "redefine_yylex.hpp"

YY_DECL;

stmt_list::ptr parse();

token get_token();

template <typename... Args>
std::optional<token_type> match_and_advance(Args... types);
template <typename... Args>
std::optional<token_type> match(Args... types);
template <typename... Args>
std::optional<token> consume(Args... types);

stmt_list::ptr program();

stmt_list::ptr block();

stmt_list::ptr statement();
stmt_list::ptr assignment();
stmt_list::ptr call();
stmt_list::ptr get();
stmt_list::ptr post();
stmt_list::ptr begin();
stmt_list::ptr condition();
stmt_list::ptr loop();

expr_tree::ptr comparison();
expr_tree::ptr expression();
expr_tree::ptr term();
expr_tree::ptr factor();

stmt_list::ptr get_last_entry_in_list(stmt_list::ptr list);