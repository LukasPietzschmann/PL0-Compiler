#pragma once

#include <iostream>
#include <optional>

#include "context.hpp"
#include "stmt_list.hpp"
#include "stack"

union expr_result {
	int num_value;
	bool bool_value;
};

bool operator==(const expr_result& lhs, const expr_result& rhs);
bool operator!=(const expr_result& lhs, const expr_result& rhs);
bool operator<(const expr_result& lhs, const expr_result& rhs);
bool operator<=(const expr_result& lhs, const expr_result& rhs);
bool operator>(const expr_result& lhs, const expr_result& rhs);
bool operator>=(const expr_result& lhs, const expr_result& rhs);
int operator+(const expr_result& lhs, const expr_result& rhs);
int operator-(const expr_result& lhs, const expr_result& rhs);
int operator*(const expr_result& lhs, const expr_result& rhs);
int operator/(const expr_result& lhs, const expr_result& rhs);
int operator%(const expr_result& lhs, int rhs);
std::ostream& operator<<(std::ostream& os, const expr_result& res);

void exec(stmt_list::ptr list);
expr_result exec(expr_tree::ptr expr);