#include "context.hpp"

context::context() { m_proc_table.try_emplace(0, (proc_table_entry){nullptr, 0}); }

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

context::error_code context::insert(const std::string& name,
		context::entry_type type,
		std::optional<int> value,
		std::shared_ptr<stmt_list> proc_start) {
	assert(type != t_unset);
	static uint32_t next_procedure_id = 1;
	int value_to_insert = value.has_value() ? *value : 0;

	const int level = m_context_table.size() - 1;
	assert(level >= 0);

	if(m_context_table[level].contains(name))
		return c_already_declared;

	if(type == t_const) {
		assert(value.has_value());
		int var_count = 0;
		for(const auto& [name, entry] : m_context_table[level]) {
			if(entry.type & t_var | t_const)
				++var_count;
		}
		assert(m_proc_table.at(m_proc_table.size() - 1).number_of_variables == var_count);
		m_proc_table.at(m_proc_table.size() - 1).number_of_variables += 1;
	} else if(type == t_var) {
		int var_count = 0;
		for(const auto& [name, entry] : m_context_table[level]) {
			if(entry.type & t_var | t_const)
				++var_count;
		}
		assert(m_proc_table.at(m_proc_table.size() - 1).number_of_variables == var_count);
		m_proc_table.at(m_proc_table.size() - 1).number_of_variables += 1;
		value_to_insert = var_count;
	} else if(t_procedure) {
		assert(proc_start != nullptr);
		value_to_insert = next_procedure_id++;
		m_proc_table.try_emplace(value_to_insert, (proc_table_entry){proc_start});
	}

	m_context_table[level][name] = context_entry(type, value_to_insert);
	return c_okay;
}

context::error_code context::lookup(const std::string& name, int type, int& out_level_delta, int& out_value) const {
	assert(type != t_unset);
	const int level = m_context_table.size() - 1;
	int current_level = level + 1;

	while(--current_level >= 0 && !m_context_table[current_level].contains(name))
		continue;

	if(current_level < 0)
		return c_not_found;

	if(const context_entry& c = m_context_table[current_level].at(name); c.type & type) {
		out_level_delta = level - current_level;
		out_value = c.value;
		return c_okay;
	} else
		return c_wrong_type;
}

const context::proc_table_entry& context::lookup_procedure(int number) const { return m_proc_table.at(number); }
int context::get_proc_count() const { return m_proc_table.size(); }

std::ostream& operator<<(std::ostream& os, const context& context) {
	const int level = context.m_context_table.size() - 1;
	for(int i = 0; i <= level; ++i) {
		for(const auto& pos : context.m_context_table[i])
			os << "Key: " << pos.first << pos.second.type << '\n';
	}

	return os;
}

void context::print() const { std::cout << *this << std::endl; }