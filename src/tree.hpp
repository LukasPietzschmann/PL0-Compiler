// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

template <typename T>
class tree {
public:
	using ptr = std::shared_ptr<tree<T>>;

	explicit tree(const T& _v);
	template <typename... Args>
	explicit tree(const T& value, Args... children);

	int size() const;

	void append(tree::ptr child);

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
	std::vector<tree<T>::ptr> m_children;
};