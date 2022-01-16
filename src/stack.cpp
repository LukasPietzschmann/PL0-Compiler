#include "stack.hpp"

stack::stack() : m_elements({0}) {}

void stack::push_sf(int var_count, int delta) {
	int new_tos = m_elements[0] + var_count + 2;
	assert(new_tos < m_elements.size() && "Stack Overflow");
	int dl = m_elements[0];
	int sl = walk_sl_chain(delta);

	m_elements[0] = new_tos;
	m_elements[new_tos] = sl;
	m_elements[new_tos - 1] = dl;
}

void stack::pop_sf() {
	assert(m_elements[0] > 0);
	m_elements[0] = m_elements[m_elements[0] - 1];
}

int stack::walk_sl_chain(int n) {
	int v = m_elements[0];
	for(int i = 0; i < n; ++i)
		v = m_elements[v];
	assert(v >= 0);
	return v;
}

void stack::set_at(int pos, int value) {
	assert(pos > 0);
	m_elements[pos] = value;
}

int stack::get_at(int pos) const {
	assert(pos > 0);
	return m_elements.at(pos);
}