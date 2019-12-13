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
#include <Basis.hpp>
//misc>

using namespace godot;

//TODO: generate random values for tests and run tests multiple times
inline float floatRand() { return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) + rand(); }

__declspec(dllimport) class godot::PlayerVelocitySystem;
__declspec(dllimport) class godot::GravitySystem;

//TODO: try to split each case into separate file
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
		playerVelSystem.Update(velocityComp, speedComp, basis, 1 << 2| 1 << 3);
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

TEST_CASE("GravitySystem test")
{
	GravitySystem system;
	GravityComponent gravityComp{ floatRand(), floatRand() };
	float delta = floatRand();

	SUBCASE("Falling down test")
	{
		VelocityComponent velocityComp{ Vector3{ 0, 0, 0 } };
		system.Update(velocityComp, gravityComp, delta, false);
		float expectedSpeed = -gravityComp.accDown * delta;

		CHECK(velocityComp.velocity.y == doctest::Approx(expectedSpeed));
	}

	SUBCASE("Flying up test")
	{
		float initialSpeed = Math::abs(floatRand());
		VelocityComponent velocityComp{ Vector3{ 0, initialSpeed, 0 } };
		system.Update(velocityComp, gravityComp, delta, false);
		float expectedSpeed = initialSpeed - gravityComp.accUp * delta;

		CHECK(velocityComp.velocity.y == doctest::Approx(expectedSpeed));
	}

	SUBCASE("Standing test")
	{
		VelocityComponent velocityComp{ Vector3{ 0, 0, 0 } };
		system.Update(velocityComp, gravityComp, delta, true);
		float expectedSpeed = -gravityComp.accDown * delta;

		CHECK(velocityComp.velocity.y == doctest::Approx(0));
	}
}