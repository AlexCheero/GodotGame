#pragma once
#include "CommonIncludes.h"

#include "core/math/math_funcs.h"

#include "Systems/GravitySystem.h"

__declspec(dllimport) class godot::GravitySystem;

TEST_CASE("GravitySystem test")
{
	GravityComponent gravityComp{ floatRand(), floatRand() };
	float delta = floatRand();

	SUBCASE("Falling down test")
	{
		VelocityComponent velocityComp{ Vector3{ 0, 0, 0 } };
		GravitySystem::Update(velocityComp, gravityComp, delta, false);
		float expectedSpeed = -gravityComp.accDown * delta;

		CHECK(velocityComp.velocity.y == doctest::Approx(expectedSpeed));
	}

	SUBCASE("Flying up test")
	{
		float initialSpeed = Math::abs(floatRand());
		VelocityComponent velocityComp{ Vector3{ 0, initialSpeed, 0 } };
		GravitySystem::Update(velocityComp, gravityComp, delta, false);
		float expectedSpeed = initialSpeed - gravityComp.accUp * delta;

		CHECK(velocityComp.velocity.y == doctest::Approx(expectedSpeed));
	}

	SUBCASE("Standing test")
	{
		VelocityComponent velocityComp{ Vector3{ 0, 0, 0 } };
		GravitySystem::Update(velocityComp, gravityComp, delta, true);
		float expectedSpeed = -gravityComp.accDown * delta;

		CHECK(velocityComp.velocity.y == doctest::Approx(0));
	}
}