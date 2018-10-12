#include <fn/function.hpp>

#include <string>

std::string foo(const std::string &lhs, const std::string &rhs) {
	const fn::Function<std::string()> f{
		[&lhs, &rhs] {
			return lhs + rhs;
		}
	};

	return f();
}
