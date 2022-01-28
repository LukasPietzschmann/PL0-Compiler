#include "optimization.hpp"

void optimize_nops(stmt_list::ptr& list) {
	stmt_list::ptr current = list;
	stmt_list::ptr next;
	stmt_list::ptr next_next;

	while(current != nullptr && next != nullptr && next_next != nullptr) {
		next = current == nullptr ? nullptr : current->get_next();
		next_next = next == nullptr ? nullptr : next->get_next();
		if(next->get_type() == stmt_list::t_nop)
			list->set_next(next_next);
		current = next;
	}

	current = list;
	stmt_list::ptr jmp;
	stmt_list::ptr jmp_next;

	while(current != nullptr && jmp != nullptr && jmp_next != nullptr) {
		current == nullptr ? nullptr : current->get_jmp();
		jmp == nullptr ? nullptr : jmp->get_next();
		if(jmp->get_type() == stmt_list::t_nop)
			list->set_jmp(jmp_next);
		current = next;
	}
}