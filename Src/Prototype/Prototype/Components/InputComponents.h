#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

//macro black magick that creates new enum value and hashed_string tag for it
#define INPUT_VALUES(value) \
	value(Attack) \
	value(Jump) \
	value(ChooseMelee) \
	value(ChooseRanged) \
	value(ChooseThrowable)

#define CREATE_ENUM_VALUE(name) \
	name,

enum class EInput
{
	INPUT_VALUES(CREATE_ENUM_VALUE)
	End
};

#define CREATE_HASH_STRING(name) \
	constexpr entt::hashed_string name##InputTag = #name##_hs;

INPUT_VALUES(CREATE_HASH_STRING)
//------------------------macro black magick ends here-----------------------

struct InputComponent
{
	std::bitset<static_cast<int>(EInput::End)> inputSet;
	//TODO?: normalize input directions in input system
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