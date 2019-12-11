#pragma once

#include <core/Godot.hpp>

namespace godot
{
	using VelocityComponent = Vector3;
	using SpeedComponent = float;

	struct GravityComponent
	{
		float accUp;
		float accDown;
	};
}