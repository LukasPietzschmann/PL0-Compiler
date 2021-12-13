// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#include <string>
#include <unordered_map>
#include <vector>

class context {
public:
	enum entry_type { t_unset = -1, t_var = 1, t_const = 2, t_procedure = 4 };
	enum error_code { c_not_found = 1, c_already_declared = 2, c_wrong_type = 4, c_okay = 8 };

	struct context_entry {
		context_entry(entry_type type = t_unset, int value = -1) : type(type), value(value) {}
		entry_type type;
		int value;
	};

	void level_up();
	void level_down();

	error_code insert(const std::string& name, entry_type type, int value);
	error_code lookup(const std::string& name, entry_type type, int& out_level_delta, int& out_value);

	void print();
	friend std::ostream& operator<<(std::ostream& os, const context& context);

private:
	std::vector<std::unordered_map<std::string, context_entry> > m_context_table;
};