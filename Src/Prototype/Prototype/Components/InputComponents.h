#pragma once

#include <core/Godot.hpp>

#include <entt/entt.hpp>

#include <bitset>

namespace godot
{
	constexpr entt::hashed_string PlayerInputTag = "PlayerInputTag"_hs;

	//TODO: move this components into InputComponent as fields
	struct InputRotationComponent { Vector2 dir; };
	struct InputVelocityComponent { Vector2 dir; };

	enum class EInput
	{
		Attack,
		Jump,

		//do not add anything after this value
		End
	};

	struct InputComponent
	{
		std::bitset<static_cast<int>(EInput::End)> inputSet;

		void Set(EInput input, bool value) { inputSet.set(static_cast<int>(input), value); }
		bool Test(EInput input) { return inputSet.test(static_cast<int>(input)); }
	};
}