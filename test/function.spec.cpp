// BSD 3-Clause License
//
// Copyright (c) 2018, Gregory Meyer
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <fn/function.hpp>

#include <string>
#include <iostream>

#include <catch2/catch.hpp>

SCENARIO("fn::Function", "[fn][fn/function.hpp][fn::Function]") {
	GIVEN("a Function created from a lambda expression") {
		const fn::Function<int(int)> foo{ [](int x) { return x * 2; } };

		WHEN("invoked") {
			const int result = foo(15);

			THEN("the result is as expected") {
				REQUIRE(result == 30);
			}
		}
	}

	GIVEN("a Function created from a value capturing lambda expression") {
		int x = 15;
		const fn::Function<int(int)> foo{ [x](int y) { return x * y; } };

		WHEN("invoked") {
			const int result = foo(15);

			THEN("the result is as expected") {
				REQUIRE(result == x * 15);
			}
		}
	}

	GIVEN("a Function created from a reference capturing lambda expression") {
		int x = 15;
		const fn::Function<int&()> foo{ [&x]() -> int& { return x; } };

		WHEN("invoked") {
			THEN("the result is as expected") {
				REQUIRE(&foo() == &x);
			}
		}
	}

	GIVEN("a Function created from a value capturing complex lambda") {
		std::string first = "foo bar";
		std::string second = "baz qux";
		const fn::Function<std::string()> foo{
			[first = std::move(first), second = std::move(second)] {
				return first + second;
			}
		};

		WHEN("invoked") {
			const auto result = foo();

			THEN("the result is as expected") {
				REQUIRE(result == "foo barbaz qux");
			}
		}
	}
}
