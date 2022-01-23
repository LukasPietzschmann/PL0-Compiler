#include "optimization.hpp"

stmt_list::ptr optimize_nops(stmt_list::ptr list) {
	stmt_list::ptr current = list;
	stmt_list::ptr next;
	stmt_list::ptr next_next;

	while(current != nullptr && next != nullptr && next_next != nullptr) {
		stmt_list::ptr next = current == nullptr ? nullptr : current->get_next();
		stmt_list::ptr next_next = next == nullptr ? nullptr : next->get_next();
		if(next->get_type() == stmt_list::t_nop)
			list->set_next(next_next);
		current = next;
	}

	current = list;
	stmt_list::ptr jmp;
	stmt_list::ptr jmp_next;

	while(current != nullptr && jmp != nullptr && jmp_next != nullptr) {
		stmt_list::ptr jmp = current == nullptr ? nullptr : current->get_jmp();
		stmt_list::ptr jmp_next = jmp == nullptr ? nullptr : jmp->get_next();
		if(jmp->get_type() == stmt_list::t_nop)
			list->set_jmp(jmp_next);
		current = next;
	}
	return list;
}