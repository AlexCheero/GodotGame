#include <Object.hpp> //hack to fix include error

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN//DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <core/Godot.hpp>

#include "core/math/math_funcs.h"

#include "Systems/PlayerFlatVelocitySystem.h"

using namespace godot;

#define CHECK_FLOAT_EQ_PRECISION(a, b, epsilon) CHECK(Math::absf(a - b) <= epsilon)
#define CHECK_FLOAT_EQ(a, b) CHECK_FLOAT_EQ_PRECISION(a, b, CMP_EPSILON)

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

	}

	SUBCASE("Move forward test")
	{

	}

	SUBCASE("Move backward test")
	{

	}

	SUBCASE("Stand still test")
	{

	}

	SUBCASE("Move left right test")
	{

	}

	SUBCASE("Move fwd bwd test")
	{

	}

	Vector2 diag(1, 1);

	SUBCASE("Move fwd left test")
	{

	}

	SUBCASE("Move fwd right test")
	{

	}

	SUBCASE("Move bwd left test")
	{

	}

	SUBCASE("Move bwd right test")
	{

	}
}