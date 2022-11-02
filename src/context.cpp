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
	for(unsigned long i = 1; i < names.size(); ++i)
		output += ", " + names[i];

	return output;
}

void context::level_up() { m_context_table.emplace_back(); }

void context::level_down() {
	const int level = static_cast<int>(m_context_table.size() - 1);
	assert(level >= 0);
	m_context_table[level].clear();
	m_context_table.pop_back();
}

context::error_code context::insert(const std::string& name,
		context::entry_type type,
		std::optional<int> value,
		std::shared_ptr<stmt_list> proc_start) {
	assert(type != t_unset);
	static int next_procedure_id = 1;
	int value_to_insert = value.has_value() ? *value : 0;

	const int level = static_cast<int>(m_context_table.size() - 1);
	assert(level >= 0);

	if(m_context_table.at(level).find(name) != m_context_table.at(level).end())
		return c_already_declared;

	if(type == t_const) {
		assert(value.has_value());
		int var_count = 0;
		for(const auto& [_, entry] : m_context_table.at(level)) {
			if(entry.type == t_var)
				++var_count;
		}
	} else if(type == t_var) {
		int var_count = 0;
		for(const auto& [_, entry] : m_context_table.at(level)) {
			if(entry.type == t_var)
				++var_count;
		}
		assert(m_proc_table.at(m_proc_table.size() - 1).number_of_variables == var_count);
		m_proc_table.at(static_cast<int>(m_proc_table.size() - 1)).number_of_variables += 1;
		value_to_insert = var_count;
	} else {  // t_procedure
		assert(proc_start != nullptr);
		value_to_insert = next_procedure_id++;
		m_proc_table.try_emplace(value_to_insert, (proc_table_entry){proc_start});
	}

	m_context_table.at(level).try_emplace(name, type, value_to_insert);
	return c_okay;
}

context::error_code context::lookup(const std::string& name,
		int type,
		int& out_level_delta,
		int& out_value,
		entry_type& out_type) const {
	assert(type != t_unset);
	const int level = static_cast<int>(m_context_table.size() - 1);
	int current_level = level + 1;

	while(--current_level >= 0 && m_context_table.at(current_level).find(name) == m_context_table.at(current_level).end())
		continue;

	if(current_level < 0)
		return c_not_found;

	if(const context_entry& c = m_context_table.at(current_level).at(name); c.type & type) {
		out_level_delta = level - current_level;
		out_value = c.value;
		out_type = c.type;
		return c_okay;
	} else
		return c_wrong_type;
}

const context::proc_table_entry& context::lookup_procedure(unsigned long number) const { return m_proc_table.at(number); }
unsigned long context::get_proc_count() const { return m_proc_table.size(); }