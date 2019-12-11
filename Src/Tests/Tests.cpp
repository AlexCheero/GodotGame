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

inline void checkFloatEq(float a, float b, float epsilon = CMP_EPSILON) { CHECK(Math::absf(a - b) <= epsilon); }
//TODO: generate random values for tests and run tests multiple times
inline float floatRand() { return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) + rand(); }

__declspec(dllimport) class godot::PlayerVelocitySystem;
__declspec(dllimport) class godot::GravitySystem;

TEST_CASE("PlayerVelocitySystem test")
{
	PlayerVelocitySystem playerVelSystem;
	VelocityComponent velocityComp;
	SpeedComponent speedComp{ floatRand() };

	SUBCASE("Move left test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1);
		checkFloatEq(velocityComp.velocity.x, -speedComp.speed);
	}

	SUBCASE("Move right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 << 1);
		checkFloatEq(velocityComp.velocity.x, speedComp.speed);
	}

	SUBCASE("Move forward test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 << 2);
		checkFloatEq(velocityComp.velocity.z, -speedComp.speed);
	}

	SUBCASE("Move backward test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 << 3);
		checkFloatEq(velocityComp.velocity.z, speedComp.speed);
	}

	SUBCASE("Stand still test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 0);
		checkFloatEq(velocityComp.velocity.x, 0);
		checkFloatEq(velocityComp.velocity.z, 0);
	}

	SUBCASE("Move left right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 | 1 << 1);
		checkFloatEq(velocityComp.velocity.x, 0);
		checkFloatEq(velocityComp.velocity.z, 0);
	}

	SUBCASE("Move fwd bwd test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 << 2| 1 << 3);
		checkFloatEq(velocityComp.velocity.x, 0);
		checkFloatEq(velocityComp.velocity.z, 0);
	}

	Vector2 diag(1, 1);
	diag.normalize();
	diag *= speedComp.speed;

	SUBCASE("Move fwd left test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 | 1 << 2);
		velocityComp.velocity.y = 0;
		checkFloatEq(velocityComp.velocity.length(), diag.length());
		checkFloatEq(velocityComp.velocity.x, -diag.x);
		checkFloatEq(velocityComp.velocity.z, -diag.y);
	}

	SUBCASE("Move fwd right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 << 2 | 1 << 1);
		velocityComp.velocity.y = 0;
		checkFloatEq(velocityComp.velocity.length(), diag.length());
		checkFloatEq(velocityComp.velocity.x, diag.x);
		checkFloatEq(velocityComp.velocity.z, -diag.y);
	}

	SUBCASE("Move bwd left test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 << 3 | 1);
		velocityComp.velocity.y = 0;
		checkFloatEq(velocityComp.velocity.length(), diag.length());
		checkFloatEq(velocityComp.velocity.x, -diag.x);
		checkFloatEq(velocityComp.velocity.z, diag.y);
	}

	SUBCASE("Move bwd right test")
	{
		playerVelSystem.Update(velocityComp, speedComp, 1 << 3 | 1 << 1);
		velocityComp.velocity.y = 0;
		checkFloatEq(velocityComp.velocity.length(), diag.length());
		checkFloatEq(velocityComp.velocity.x, diag.x);
		checkFloatEq(velocityComp.velocity.z, diag.y);
	}
}

TEST_CASE("GravitySystem test")
{
	GravitySystem system;
	GravityComponent gravityComp{ 30, 20 };
	float delta = 1.f;

	SUBCASE("Falling down test")
	{
		VelocityComponent velocityComp{ Vector3{ 0, 0, 0 } };
		system.Update(velocityComp, gravityComp, delta, false);
		float expectedSpeed = -gravityComp.accDown * delta;
		checkFloatEq(velocityComp.velocity.y, expectedSpeed);
	}

	SUBCASE("Flying up test")
	{
		const float initialSpeed = 10;
		VelocityComponent velocityComp{ Vector3{ 0, 0, 0 } };
		system.Update(velocityComp, gravityComp, delta, false);
		float expectedSpeed = initialSpeed - gravityComp.accUp;
		checkFloatEq(velocityComp.velocity.y, expectedSpeed);
	}

	SUBCASE("Standing test")
	{
		VelocityComponent velocityComp{ Vector3{ 0, 0, 0 } };
		system.Update(velocityComp, gravityComp, delta, true);
		float expectedSpeed = -gravityComp.accDown * delta;
		checkFloatEq(velocityComp.velocity.y, 0);
	}
}