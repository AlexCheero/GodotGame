#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

#include "ComponentsMeta.h"

ECS_EVENT(AttackPressedTag);
ECS_EVENT(JumpPressedTag);
ECS_EVENT(ChooseMeleePressedTag);
ECS_EVENT(ChooseRangedPressedTag);
ECS_EVENT(ChooseThrowablePressedTag);
ECS_EVENT(ChooseGrenadePressedTag);

struct MoveDirInputComponent { godot::Vector2 dir; };
struct RotationInputComponent { godot::Vector2 dir; };