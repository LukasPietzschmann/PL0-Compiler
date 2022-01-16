#pragma once

#include <initializer_list>
#include <optional>
#include <string>

#include "context.hpp"
#include "oplist.hpp"
#include "optimization.hpp"
#include "redefine_yylex.hpp"

YY_DECL;

oplist::ptr parse();

token get_token();

template <typename... Args>
std::optional<token_type> match_and_advance(Args... types);
template <typename... Args>
std::optional<token_type> match(Args... types);
template <typename... Args>
std::optional<token> consume(Args... types);

oplist::ptr program();

oplist::ptr block();

oplist::ptr statement();
oplist::ptr assignment();
oplist::ptr call();
oplist::ptr get();
oplist::ptr post();
oplist::ptr begin();
oplist::ptr condition();
oplist::ptr loop();

expr_tree::ptr comparison();
expr_tree::ptr expression();
expr_tree::ptr term();
expr_tree::ptr factor();

oplist::ptr get_last_entry_in_list(oplist::ptr list);