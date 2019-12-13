#pragma once
#include "CommonIncludes.h"

#include "Systems/PlayerRotationSystem.h"

//TODO: find a way to proper test this system
TEST_CASE("PlayerRotationSystem test")
{
	PlayerRotationSystem system;
	Vector2 inputDir{ floatRand(), floatRand() };
	Basis basis(floatRand(), floatRand(), floatRand(),
				floatRand(), floatRand(), floatRand(),
				floatRand(), floatRand(), floatRand());

	Vector3 expectedDir{ 0, 0, 0 };
	expectedDir += basis.x * inputDir.x + basis.z * inputDir.y;
	expectedDir.y = 0;

	Vector3 realDir = system.GetTargetDirection(inputDir, basis);

	CHECK(expectedDir.length() == doctest::Approx(realDir.length()));
	CHECK(expectedDir.x == doctest::Approx(realDir.x));
	CHECK(expectedDir.y == doctest::Approx(realDir.y));
	CHECK(expectedDir.z == doctest::Approx(realDir.z));
}