#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

#include "ComponentsMeta.h"

DECLARE_TAG(AttackPressedTag);
DECLARE_TAG(JumpPressedTag);
DECLARE_TAG(ChooseMeleePressedTag);
DECLARE_TAG(ChooseRangedPressedTag);
DECLARE_TAG(ChooseThrowablePressedTag);
DECLARE_TAG(ChooseGrenadePressedTag);

struct MoveDirInputComponent { godot::Vector2 dir; };
struct RotationInputComponent { godot::Vector2 dir; };