#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

namespace godot
{
	constexpr entt::hashed_string PlayerInputTag = "PlayerInputTag"_hs;

	//TODO: move this components into InputComponent as fields
	struct InputRotationComponent { Vector2 dir; };
	struct InputVelocityComponent { Vector2 dir; };

	//TODO: make input bitmask
	struct InputComponent
	{
		bool attack;
		bool jump;
	};
}