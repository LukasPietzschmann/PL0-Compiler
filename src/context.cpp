// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#include "context.hpp"

#include <iostream>
#include <string>

context& context::the() {
	static context instance;
	return instance;
}

void context::level_up() { m_context_table.emplace_back(); }

void context::level_down() {
	const unsigned long level = m_context_table.size() - 1;
	assert(level >= 0);
	m_context_table[level].clear();
	m_context_table.pop_back();
}

context::error_code context::insert(const std::string& name, context::entry_type type, int value) {
	const unsigned long level = m_context_table.size() - 1;
	assert(level >= 0);

	if(m_context_table[level].contains(name))
		return c_already_declared;

	m_context_table[level][name] = context_entry(type, value);
	return c_okay;
}

context::error_code context::lookup(const std::string& name, int type, unsigned long& out_level_delta, int& out_value) {
	assert(type != t_unset);
	const unsigned long level = m_context_table.size() - 1;
	unsigned long current_level = level + 1;
	out_level_delta = -1;

	while(--current_level >= 0 && !m_context_table[current_level].contains(name))
		continue;

	if(current_level < 0)
		return c_not_found;

	if(const context_entry& c = m_context_table[current_level][name]; c.type & type) {
		out_level_delta = level - current_level;
		out_value = c.value;
		return c_okay;
	} else
		return c_wrong_type;
}

std::ostream& operator<<(std::ostream& os, const context& context) {
	const unsigned long level = context.m_context_table.size() - 1;
	for(int i = 0; i <= level; ++i) {
		for(const auto& pos : context.m_context_table[i])
			os << "Key: " << pos.first << pos.second.type << '\n';
	}

	return os;
}

void context::print() { std::cout << *this << std::endl; }