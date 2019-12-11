#include <Object.hpp> //hack to fix include error

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN//DOCTEST_CONFIG_IMPLEMENT to implement explicitly
#include "doctest.h"

//<godot bindings
#include <core/Godot.hpp>
//godot bindings>

//<godot source
#include "core/math/math_funcs.h"
//godot source>

//<scripts
#include "Systems/PlayerVelocitySystem.h"
#include "Systems/GravitySystem.h"
//scripts>

//<misc
#include <cstdlib>
//misc>

using namespace godot;

//TODO: implement with inline functions
#define CHECK_FLOAT_EQ_PRECISION(a, b, epsilon) CHECK(Math::absf(a - b) <= epsilon)
#define CHECK_FLOAT_EQ(a, b) CHECK_FLOAT_EQ_PRECISION(a, b, CMP_EPSILON)
//TODO: generate random values for tests and run tests multiple times
#define FLOAT_RAND() static_cast<float>(rand()) / static_cast<float>(RAND_MAX) + rand()

__declspec(dllimport) class godot::PlayerVelocitySystem;
__declspec(dllimport) class godot::GravitySystem;

TEST_CASE("PlayerVelocitySystem test")
{
	PlayerVelocitySystem playerVelSystem;
	VelocityComponent vel;
	SpeedComponent speed = 1.f;
	float delta = 1.f;

	SUBCASE("Move left test")
	{
		playerVelSystem.Update(vel, speed, delta, 1);
		CHECK_FLOAT_EQ(vel.x, -speed * delta);
	}

	SUBCASE("Move right test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 << 1);
		CHECK_FLOAT_EQ(vel.x, speed * delta);
	}

	SUBCASE("Move forward test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 << 2);
		CHECK_FLOAT_EQ(vel.z, -speed * delta);
	}

	SUBCASE("Move backward test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 << 3);
		CHECK_FLOAT_EQ(vel.z, speed * delta);
	}

	SUBCASE("Stand still test")
	{
		playerVelSystem.Update(vel, speed, delta, 0);
		CHECK_FLOAT_EQ(vel.x, 0);
		CHECK_FLOAT_EQ(vel.z, 0);
	}

	SUBCASE("Move left right test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 | 1 << 1);
		CHECK_FLOAT_EQ(vel.x, 0);
		CHECK_FLOAT_EQ(vel.z, 0);
	}

	SUBCASE("Move fwd bwd test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 << 2| 1 << 3);
		CHECK_FLOAT_EQ(vel.x, 0);
		CHECK_FLOAT_EQ(vel.z, 0);
	}

	Vector2 diag(1, 1);
	diag.normalize();

	SUBCASE("Move fwd left test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 | 1 << 2);
		vel.y = 0;
		CHECK_FLOAT_EQ(vel.length(), diag.length());
		CHECK_FLOAT_EQ(vel.x, -diag.x);
		CHECK_FLOAT_EQ(vel.z, -diag.y);
	}

	SUBCASE("Move fwd right test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 << 2 | 1 << 1);
		vel.y = 0;
		CHECK_FLOAT_EQ(vel.length(), diag.length());
		CHECK_FLOAT_EQ(vel.x, diag.x);
		CHECK_FLOAT_EQ(vel.z, -diag.y);
	}

	SUBCASE("Move bwd left test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 << 3 | 1);
		vel.y = 0;
		CHECK_FLOAT_EQ(vel.length(), diag.length());
		CHECK_FLOAT_EQ(vel.x, -diag.x);
		CHECK_FLOAT_EQ(vel.z, diag.y);
	}

	SUBCASE("Move bwd right test")
	{
		playerVelSystem.Update(vel, speed, delta, 1 << 3 | 1 << 1);
		vel.y = 0;
		CHECK_FLOAT_EQ(vel.length(), diag.length());
		CHECK_FLOAT_EQ(vel.x, diag.x);
		CHECK_FLOAT_EQ(vel.z, diag.y);
	}
}

TEST_CASE("GravitySystem test")
{
	GravitySystem system;
	GravityComponent gravityComp{ 30, 20 };
	float delta = 1.f;

	SUBCASE("Falling down test")
	{
		Vector3 velocity{ 0, 0, 0 };
		system.Update(velocity, gravityComp, delta, false);
		float expectedSpeed = -gravityComp.accDown * delta;
		CHECK_FLOAT_EQ(velocity.y, expectedSpeed);
	}

	SUBCASE("Flying up test")
	{
		const float initialSpeed = 10;
		Vector3 velocity{ 0, initialSpeed, 0 };
		system.Update(velocity, gravityComp, delta, false);
		float expectedSpeed = initialSpeed - gravityComp.accUp;
		CHECK_FLOAT_EQ(velocity.y, expectedSpeed);
	}

	SUBCASE("Standing test")
	{
		Vector3 velocity{ 0, 0, 0 };
		system.Update(velocity, gravityComp, delta, true);
		float expectedSpeed = -gravityComp.accDown * delta;
		CHECK_FLOAT_EQ(velocity.y, 0);
	}
}

//TODO: try to dllimport godot bindings and test KinematicMovementSystem