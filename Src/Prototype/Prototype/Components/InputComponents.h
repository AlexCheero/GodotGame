#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

namespace godot
{
	constexpr entt::hashed_string PlayerInputTag = "PlayerInputTag"_hs;

	enum class EInput
	{
		Attack,
		Jump,
		ChooseMelee,
		ChooseRanged,
		ChooseThrowable,

		//do not add anything after this value
		End
	};

	struct InputComponent
	{
		std::bitset<static_cast<int>(EInput::End)> inputSet;
		Vector2 rotation;
		Vector2 moveDir;

		void Set(EInput input, bool value) { inputSet.set(static_cast<int>(input), value); }
		bool Test(EInput input) { return inputSet.test(static_cast<int>(input)); }
		bool TestAndReset(EInput input)
		{ 
			int intInput = static_cast<int>(input);
			bool value = inputSet.test(intInput);
			inputSet.set(intInput, false);
			return value;
		}
	};
}