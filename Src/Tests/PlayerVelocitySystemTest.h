#pragma once
#include "CommonIncludes.h"

#include "Systems/PlayerVelocitySystem.h"

#include <Basis.hpp>

__declspec(dllimport) class godot::PlayerVelocitySystem;

TEST_CASE("PlayerVelocitySystem test")
{
	VelocityComponent velocityComp;
	SpeedComponent speedComp{ floatRand() };
	Basis basis(Vector3{ 1, 0, 0 }, Vector3{ 0, 1, 0 }, Vector3{ 0, 0, 1 });

	SUBCASE("Move left test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ 1.f, 0.f });
		CHECK(velocityComp.velocity.x == doctest::Approx(-speedComp.speed));
	}

	SUBCASE("Move right test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ -1.f, 0.f });
		CHECK(velocityComp.velocity.x == doctest::Approx(speedComp.speed));
	}

	SUBCASE("Move forward test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ 0.f, 1.f });
		CHECK(velocityComp.velocity.z == doctest::Approx(-speedComp.speed));
	}

	SUBCASE("Move backward test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ 0.f, -1.f });
		CHECK(velocityComp.velocity.z == doctest::Approx(speedComp.speed));
	}

	SUBCASE("Stand still test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ 0.f, 0.f });
		CHECK(velocityComp.velocity.x == doctest::Approx(0));
		CHECK(velocityComp.velocity.z == doctest::Approx(0));
	}

	Vector2 diag(1, 1);
	diag.normalize();
	diag *= speedComp.speed;

	SUBCASE("Move fwd left test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ 1.f, 1.f });
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(-diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(-diag.y));
	}

	SUBCASE("Move fwd right test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ -1.f, 1.f });
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(-diag.y));
	}

	SUBCASE("Move bwd left test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ 1.f, -1.f });
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(-diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(diag.y));
	}

	SUBCASE("Move bwd right test")
	{
		PlayerVelocitySystem::Update(velocityComp, speedComp, basis, Vector2{ -1.f, -1.f });
		velocityComp.velocity.y = 0;

		CHECK(velocityComp.velocity.length() == doctest::Approx(diag.length()));
		CHECK(velocityComp.velocity.x == doctest::Approx(diag.x));
		CHECK(velocityComp.velocity.z == doctest::Approx(diag.y));
	}
}