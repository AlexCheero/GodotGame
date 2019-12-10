#include <Object.hpp> //hack to fix include error

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN//DOCTEST_CONFIG_IMPLEMENT to implement explicitly
#include "doctest.h"

#include <core/Godot.hpp>

#include "core/math/math_funcs.h"

#include "Systems/PlayerFlatVelocitySystem.h"

using namespace godot;

#define CHECK_FLOAT_EQ_PRECISION(a, b, epsilon) CHECK(Math::absf(a - b) <= epsilon)
#define CHECK_FLOAT_EQ(a, b) CHECK_FLOAT_EQ_PRECISION(a, b, CMP_EPSILON)

__declspec(dllimport) class godot::PlayerFlatVelocitySystem;

TEST_CASE("PlayerFlatVelocitySystem test")
{
	PlayerFlatVelocitySystem pfvs;
	FlatVelocityComponent flatVel;
	SpeedComponent speed = 1.f;
	float delta = 1.f;

	SUBCASE("Move left test")
	{
		pfvs.Update(flatVel, speed, delta, 1);
		CHECK_FLOAT_EQ(flatVel.x, -speed * delta);
	}

	SUBCASE("Move right test")
	{
		pfvs.Update(flatVel, speed, delta, 1 << 1);
		CHECK_FLOAT_EQ(flatVel.x, speed * delta);
	}

	SUBCASE("Move forward test")
	{
		pfvs.Update(flatVel, speed, delta, 1 << 2);
		CHECK_FLOAT_EQ(flatVel.y, -speed * delta);
	}

	SUBCASE("Move backward test")
	{
		pfvs.Update(flatVel, speed, delta, 1 << 3);
		CHECK_FLOAT_EQ(flatVel.y, speed * delta);
	}

	SUBCASE("Stand still test")
	{
		pfvs.Update(flatVel, speed, delta, 0);
		CHECK_FLOAT_EQ(flatVel.x, 0);
		CHECK_FLOAT_EQ(flatVel.y, 0);
	}

	SUBCASE("Move left right test")
	{
		pfvs.Update(flatVel, speed, delta, 1 | 1 << 1);
		CHECK_FLOAT_EQ(flatVel.x, 0);
		CHECK_FLOAT_EQ(flatVel.y, 0);
	}

	SUBCASE("Move fwd bwd test")
	{
		pfvs.Update(flatVel, speed, delta, 1 << 2| 1 << 3);
		CHECK_FLOAT_EQ(flatVel.x, 0);
		CHECK_FLOAT_EQ(flatVel.y, 0);
	}

	Vector2 diag(1, 1);
	diag.normalize();

	SUBCASE("Move fwd left test")
	{
		pfvs.Update(flatVel, speed, delta, 1 | 1 << 2);
		CHECK_FLOAT_EQ(flatVel.length(), diag.length());
		CHECK_FLOAT_EQ(flatVel.x, -diag.x);
		CHECK_FLOAT_EQ(flatVel.y, -diag.y);
	}

	SUBCASE("Move fwd right test")
	{
		pfvs.Update(flatVel, speed, delta, 1 << 2 | 1 << 1);
		CHECK_FLOAT_EQ(flatVel.length(), diag.length());
		CHECK_FLOAT_EQ(flatVel.x, diag.x);
		CHECK_FLOAT_EQ(flatVel.y, -diag.y);
	}

	SUBCASE("Move bwd left test")
	{
		pfvs.Update(flatVel, speed, delta, 1 << 3 | 1);
		CHECK_FLOAT_EQ(flatVel.length(), diag.length());
		CHECK_FLOAT_EQ(flatVel.x, -diag.x);
		CHECK_FLOAT_EQ(flatVel.y, diag.y);
	}

	SUBCASE("Move bwd right test")
	{
		pfvs.Update(flatVel, speed, delta, 1 << 3 | 1 << 1);
		CHECK_FLOAT_EQ(flatVel.length(), diag.length());
		CHECK_FLOAT_EQ(flatVel.x, diag.x);
		CHECK_FLOAT_EQ(flatVel.y, diag.y);
	}
}