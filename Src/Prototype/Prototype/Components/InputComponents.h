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