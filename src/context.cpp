// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#include "context.hpp"

context& context::the() {
	static context instance;
	return instance;
}

std::string context::get_name_for_type(int entry_type) {
	std::vector<std::string> names;
	names.reserve(2);
	if(entry_type & t_var)
		names.emplace_back("Variable");
	if(entry_type & t_const)
		names.emplace_back("Constant");
	if(entry_type & t_procedure)
		names.emplace_back("Procedure");

	std::string output = names[0];
	for(int i = 1; i < names.size(); ++i)
		output += ", " + names[i];

	return output;
}

void context::level_up() { m_context_table.emplace_back(); }

void context::level_down() {
	const int level = m_context_table.size() - 1;
	assert(level >= 0);
	m_context_table[level].clear();
	m_context_table.pop_back();
}

context::error_code context::insert(const std::string& name, context::entry_type type, int value) {
	const int level = m_context_table.size() - 1;
	assert(level >= 0);

	if(m_context_table[level].contains(name))
		return c_already_declared;

	m_context_table[level][name] = context_entry(type, value);
	return c_okay;
}

context::error_code context::lookup(const std::string& name, int type, int& out_level_delta, int& out_value) {
	assert(type != t_unset);
	const int level = m_context_table.size() - 1;
	int current_level = level + 1;

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
	const int level = context.m_context_table.size() - 1;
	for(int i = 0; i <= level; ++i) {
		for(const auto& pos : context.m_context_table[i])
			os << "Key: " << pos.first << pos.second.type << '\n';
	}

	return os;
}

void context::print() const { std::cout << *this << std::endl; }