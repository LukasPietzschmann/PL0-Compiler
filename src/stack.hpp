#pragma once

#include <array>
#include <ostream>

#include "context.hpp"

class stack {
public:
	static stack& the() {
		static stack instance;
		return instance;
	}

	stack(const stack&) = delete;
	stack(stack&&) noexcept = default;
	stack& operator=(const stack&) = delete;
	stack& operator=(stack&&) noexcept = default;

	void push_sf(int var_count, int delta);
	void pop_sf();

	int walk_sl_chain(int n);

	void set_at(int pos, int value);
	int get_at(int pos) const;

private:
	stack();
	std::array<int, 100> m_elements;
};