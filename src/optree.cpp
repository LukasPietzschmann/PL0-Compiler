// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#include "tree.hpp"

template <typename T>
tree<T>::tree(const T& value) : m_value(value) {}

template <typename T>
template <typename... Args>
tree<T>::tree(const T& value, Args... children) : m_value(value), m_children({children...}) {}

template <typename T>
int tree<T>::size() const {
	return m_children.size();
}

template <typename T>
void tree<T>::append(tree::ptr child) {
	m_children.push_back(child);
}

template <typename T>
void tree<T>::set_value(const T& value) {
	m_value = value;
}

template <typename T>
T& tree<T>::get_value() const {
	return m_value;
}

template <typename T>
void tree<T>::overwrite_children(tree::ptr t) {
	m_children = t->m_children;
}

template <typename T>
template <typename Callback>
void tree<T>::execute_in_postorder(const Callback& callback) const {
	for(const tree<T>::ptr child : m_children)
		child->execute_in_postorder(callback);
	callback(*this);
}

template <typename T>
void tree<T>::print_in_postorder(std::ostream& os) const {
	execute_in_postorder([&os](const tree<T>& tree) { os << tree.m_value << " "; });
}

template <typename T>
std::optional<tree<T>> tree<T>::operator[](int index) const {
	if(index < 0 || index >= m_children.size())
		return {};
	else
		return m_children[index];
}

template <typename T>
template <typename RT>
std::shared_ptr<RT> inline tree<T>::as(tree::ptr tree) {
	return std::dynamic_pointer_cast<RT>(tree);
}