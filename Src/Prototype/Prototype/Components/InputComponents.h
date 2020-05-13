#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

#include "ComponentsMeta.h"

#define GET_AXIS_TYPES(Axis) Axis ## ChangedTag, Axis ## InputComponent
#define DECLARE_AXIS(Axis) DECLARE_TAG(Axis ## ChangedTag); \
                           struct Axis ## InputComponent { godot::Vector2 dir; }

DECLARE_TAG(AttackPressedTag);
DECLARE_TAG(JumpPressedTag);
DECLARE_TAG(ChooseMeleePressedTag);
DECLARE_TAG(ChooseRangedPressedTag);
DECLARE_TAG(ChooseThrowablePressedTag);

DECLARE_AXIS(Rotation);
DECLARE_AXIS(MoveDir);

//DECLARE_TAG(RotationChangedtag);
//struct RotationInput { godot::Vector2 dir; };
//DECLARE_TAG(MoveDirChangedtag);
//struct MoveDirInput { godot::Vector2 dir; };

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

//TODO0: remove or use only for network synchronization if needed
struct InputComponent
{
	std::bitset<static_cast<int>(EInput::End)> inputSet;
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