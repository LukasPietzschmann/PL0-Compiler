#include "optimization.hpp"

oplist::ptr optimize_nops(oplist::ptr list) {
	oplist::ptr current = list;
	oplist::ptr next;
	oplist::ptr next_next;

	while(current != nullptr && next != nullptr && next_next != nullptr) {
		oplist::ptr next = current == nullptr ? nullptr : current->get_next();
		oplist::ptr next_next = next == nullptr ? nullptr : next->get_next();
		if(next->get_type() == oplist::t_nop)
			list->set_next(next_next);
		current = next;
	}

	current = list;
	oplist::ptr jmp;
	oplist::ptr jmp_next;

	while(current != nullptr && jmp != nullptr && jmp_next != nullptr) {
		oplist::ptr jmp = current == nullptr ? nullptr : current->get_jmp();
		oplist::ptr jmp_next = jmp == nullptr ? nullptr : jmp->get_next();
		if(jmp->get_type() == oplist::t_nop)
			list->set_jmp(jmp_next);
		current = next;
	}
	return list;
}