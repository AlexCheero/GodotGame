#pragma once

#include <core/Godot.hpp>

namespace godot
{
	using FlatVelocityComponent = Vector2;
	using SpeedComponent = float;

	struct GravityComponent
	{
		float speed;
		float accUp;
		float accDown;
		float jumpSpeed;
	};
}