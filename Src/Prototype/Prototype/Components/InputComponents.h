#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

#include "ComponentsMeta.h"

TAG(AttackPressedTag);
TAG(JumpPressedTag);
TAG(ChooseMeleePressedTag);
TAG(ChooseRangedPressedTag);
TAG(ChooseThrowablePressedTag);
TAG(ChooseGrenadePressedTag);

struct MoveDirInputComponent { godot::Vector2 dir; };
struct RotationInputComponent { godot::Vector2 dir; };