#pragma once
#include "CommonIncludes.h"

#include "Systems/JumpSystem.h"

__declspec(dllimport) class godot::JumpSystem;

TEST_CASE("JumpSystem test")
{
	VelocityComponent velocity{ Vector3{ floatRand(), floatRand(), floatRand() } };
	JumpSpeedComponent jump{ floatRand() };
	
	JumpSystem::Update(velocity, jump);

	CHECK(velocity.velocity.y == doctest::Approx(jump.speed));
}