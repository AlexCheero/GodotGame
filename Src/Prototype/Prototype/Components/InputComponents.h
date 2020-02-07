#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

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
	//TODO: normalize input directions in input system
	godot::Vector2 rotation;
	godot::Vector2 moveDir;

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