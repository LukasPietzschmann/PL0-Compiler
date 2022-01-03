// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

class tree {
public:
	struct entry {
		enum entry_type {
			t_unset,
			t_block,
			t_const,
			t_var,
			t_proc,
			t_assignment,
			t_call,
			t_get,
			t_post,
			t_begin,
			t_if,
			t_loop,
			t_odd,
			t_equal,
			t_not_equal,
			t_less,
			t_less_equal,
			t_greater,
			t_greater_equal,
			t_plus,
			t_minus,
			t_start,
			t_slash,
			t_identifier,
			t_literal
		};
	};

	explicit tree(const T& _v);
	template <typename... Args>
	explicit tree(const T& value, Args... children);

	int size() const;

	void append(tree child);
	template <typename... Args>
	void append(Args... args);

	void set_value(const T& value);
	T& get_value() const;

	void overwrite_children(tree::ptr child_tree);

	template <typename Callback>
	void execute_in_postorder(const Callback& callback) const;
	void print_in_postorder(std::ostream& os = std::cout) const;

	std::optional<tree<T>> operator[](int index) const;

	template <typename RT>
	static std::shared_ptr<RT> as(tree<T>::ptr tree);

protected:
	T m_value;
	std::vector<tree> m_children;
};