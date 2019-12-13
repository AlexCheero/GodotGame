#pragma once
#include "CommonIncludes.h"

#include "core/math/math_funcs.h"

#include "Systems/CameraFollowSystem.h"

//TODO: find a way to proper test this system
TEST_CASE("CameraFollowSystem test")
{
	const Vector3 globalX = Vector3{ 1, 0, 0 };
	const Vector3 globalY = Vector3{ 0, 1, 0 };
	const Vector3 globalZ = Vector3{ 0, 0, 1 };

	CameraFollowSystem system;
	Vector3 targetPosition{ floatRand(), floatRand(), floatRand() };
	CamPositionComponent relPos{ floatRand(), floatRand(), floatRand() };

	Vector3 expectedCamDirection = globalZ.rotated(globalY, Math::deg2rad(relPos.yAngle));
	expectedCamDirection = expectedCamDirection.rotated(globalX, Math::deg2rad(relPos.xAngle));
	Vector3 expectedCamPosition = targetPosition + expectedCamDirection.normalized() * relPos.distance;
	Vector3 realCamPosition = system.NewCamPosition(targetPosition, relPos);
	
	CHECK(expectedCamPosition.length() == doctest::Approx(realCamPosition.length()));
	CHECK(expectedCamPosition.x == doctest::Approx(realCamPosition.x));
	CHECK(expectedCamPosition.y == doctest::Approx(realCamPosition.y));
	CHECK(expectedCamPosition.z == doctest::Approx(realCamPosition.z));
}
