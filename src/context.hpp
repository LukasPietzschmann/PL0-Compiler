// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "logger.hpp"

class context {
public:
	enum entry_type { t_unset = -1, t_var = 1, t_const = 2, t_procedure = 4 };
	enum error_code { c_not_found = 1, c_already_declared = 2, c_wrong_type = 4, c_okay = 8 };

	static std::string get_name_for_type(int entry_type);

	struct context_entry {
		explicit context_entry(entry_type type = t_unset, int value = -1) : type(type), value(value) {}
		entry_type type;
		int value;
	};

	context& operator=(const context&) = delete;
	context& operator=(context&&) = default;
	context(const context&) = delete;
	context(context&&) = default;

	static context& the();

	void level_up();
	void level_down();

	error_code insert(const std::string& name, entry_type type, int value = 0);
	error_code lookup(const std::string& name, int type, unsigned long& out_level_delta, int& out_value);

	void print() const;
	friend std::ostream& operator<<(std::ostream& os, const context& context);

private:
	context() = default;
	std::vector<std::unordered_map<std::string, context_entry> > m_context_table;
};