#include "vm_emitter.hpp"

void vm_emitter::emit(emitter::token_type type,
		std::optional<int> value,
		std::optional<std::string_view> target_label,
		std::optional<std::string_view> src_label,
		std::string_view comment) {
	if(src_label.has_value())
		m_ofs << *src_label << " ";
	else
		m_ofs << "\t";
	switch(type) {
		case t_add: m_ofs << "add"; break;
		case t_sub: m_ofs << "sub"; break;
		case t_mult: m_ofs << "mult"; break;
		case t_div: m_ofs << "div"; break;
		case t_mod: m_ofs << "mod"; break;
		case t_chs: m_ofs << "chs"; break;
		case t_cmpeq: m_ofs << "cmpeq"; break;
		case t_cmpne: m_ofs << "cmpne"; break;
		case t_cmple: m_ofs << "cmple"; break;
		case t_cmplt: m_ofs << "cmplt"; break;
		case t_cmpge: m_ofs << "cmpge"; break;
		case t_cmpgt: m_ofs << "cmpgt"; break;
		case t_jmp: m_ofs << "jump " << *target_label; break;
		case t_jmpz: m_ofs << "jumpz " << *target_label; break;
		case t_jmpnz: m_ofs << "jumpnz " << *target_label; break;
		case t_call: m_ofs << "call " << *target_label; break;
		case t_return: m_ofs << "return"; break;
		case t_loadc: m_ofs << "loadc " << *value; break;
		case t_loadr: m_ofs << "loadr " << *value; break;
		case t_loads: m_ofs << "loads"; break;
		case t_storer: m_ofs << "storer " << *value; break;
		case t_stores: m_ofs << "stores"; break;
		case t_inc: m_ofs << "inc " << *value; break;
		case t_dec: m_ofs << "dec " << *value; break;
		case t_dup: m_ofs << "dup"; break;
		case t_drop: m_ofs << "drop"; break;
		case t_swap: m_ofs << "swap"; break;
		case t_read: m_ofs << "read"; break;
		case t_write: m_ofs << "write"; break;
		case t_nop:
			if(src_label.has_value() || !comment.empty())
				m_ofs << "nop";
			break;
	}

	if(!comment.empty())
		m_ofs << "\t\t" << "# " << comment;

	m_ofs << "\n";
}