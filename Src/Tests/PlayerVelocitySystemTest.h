#pragma once
#include "CommonIncludes.h"

#include "Systems/PlayerVelocitySystem.h"

#include <Basis.hpp>

__declspec(dllimport) class godot::PlayerVelocitySystem;

TEST_CASE("PlayerVelocitySystem test")
{
	PlayerVelocitySystem playerVelSystem;
	VelocityComponent velocityComp;
	SpeedComponent speedComp{ floatRand() };
	Basis basis(Vector3{ 1, 0, 0 }, Vector3{ 0, 1, 0 }, Vector3{ 0, 0, 1 });

	SUBCASE("Move left test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1);
		CHECK(velocityComp.velocity.x == doctest::Approx(-speedComp.speed));
	}

	SUBCASE("Move right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 1);
		CHECK(velocityComp.velocity.x == doctest::Approx(speedComp.speed));
	}

	SUBCASE("Move forward test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 2);
		CHECK(velocityComp.velocity.z == doctest::Approx(-speedComp.speed));
	}

	SUBCASE("Move backward test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 3);
		CHECK(velocityComp.velocity.z == doctest::Approx(speedComp.speed));
	}

	SUBCASE("Stand still test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 0);
		CHECK(velocityComp.velocity.x == doctest::Approx(0));
		CHECK(velocityComp.velocity.z == doctest::Approx(0));
	}

	SUBCASE("Move left right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 | 1 << 1);
		CHECK(velocityComp.velocity.x == doctest::Approx(0));
		CHECK(velocityComp.velocity.z == doctest::Approx(0));
	}

	SUBCASE("Move fwd bwd test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 2 | 1 << 3);
		CHECK(velocityComp.velocity.x == doctest::Approx(0));
		CHECK(velocityComp.velocity.z == doctest::Approx(0));
	}

	Vector2 diag(1, 1);
	diag.normalize();
	diag *= speedComp.speed;

	SUBCASE("Move fwd left test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 | 1 << 2);
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(-diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(-diag.y));
	}

	SUBCASE("Move fwd right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 2 | 1 << 1);
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(-diag.y));
	}

	SUBCASE("Move bwd left test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 3 | 1);
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(-diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(diag.y));
	}

	SUBCASE("Move bwd right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 3 | 1 << 1);
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(diag.y));
	}
}