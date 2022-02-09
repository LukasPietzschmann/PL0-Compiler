#pragma once

#include <fstream>
#include <optional>
#include <string_view>

class emitter {
public:
	enum token_type {
		// clang-format off
		t_add, t_sub, t_mult, t_div, t_mod,
		t_chs,
		t_cmpeq, t_cmpne, t_cmple, t_cmplt, t_cmpge, t_cmpgt,
		t_jmp, t_jmpz, t_jmpnz,
		t_call, t_return,
		t_loadc, t_loadr, t_loads,
		t_storer, t_stores,
		t_inc, t_dec, t_dup, t_drop, t_swap,
		t_read, t_write,
		t_nop
		// clang-format on
	};

	explicit emitter(std::ofstream stream_to_emit_to) : m_ofs(std::forward<std::ofstream>(stream_to_emit_to)) {}
	~emitter() { m_ofs.flush(); }

	virtual void emit(token_type type,
			std::optional<int> value = {},
			std::optional<std::string_view> target_label = {},
			std::optional<std::string_view> src_label = {},
			std::string_view comment = "") = 0;

protected:
	std::ofstream m_ofs;
};