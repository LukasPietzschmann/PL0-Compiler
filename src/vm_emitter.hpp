#include "emitter.hpp"

#pragma once

class vm_emitter : public emitter {
public:
	using emitter::emitter;

	virtual void emit(token_type type,
			std::optional<int> value = {},
			std::optional<std::string_view> target_label = {},
			std::optional<std::string_view> src_label = {},
			std::string_view comment = "") override;
};