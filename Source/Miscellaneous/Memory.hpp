#pragma once

#include <memory>

namespace dty {
	// Ref = Reference count pointer
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Arguments>
	constexpr Ref<T> CreateRef(Arguments&& ... arguments) {
		return std::make_shared<T>(std::forward<Arguments>(arguments)...);
	}
}