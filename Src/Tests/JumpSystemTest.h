#pragma once
#include "CommonIncludes.h"

#include "Systems/JumpSystem.h"

__declspec(dllimport) class godot::JumpSystem;

TEST_CASE("JumpSystem test")
{
	JumpSystem system;
	VelocityComponent velocity{ Vector3{ floatRand(), floatRand(), floatRand() } };
	JumpSpeedComponent jump{ floatRand() };
	
	system.Update(velocity, jump);

	CHECK(velocity.velocity.y == doctest::Approx(jump.speed));
}