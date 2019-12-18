#pragma once
#include "CommonIncludes.h"

#include "core/math/math_funcs.h"

#include "Systems/AttackSystem.h"

__declspec(dllimport) class godot::AttackSystem;

TEST_CASE("AttackSystem test")
{
	Vector3 attackerPosition{ floatRand(), floatRand(), floatRand() };
	//TODO: try to randomize y
	Vector3 attackerDirection{ floatRand(), 0, floatRand() };
	attackerDirection.normalize();
	float maxAngle = floatRandExcludeHighBound(180.f);

	float angleIn = floatRandIncludeHighBound(0, maxAngle);
	CHECK(angleIn < maxAngle);
	float cosIn = Math::cos(Math::deg2rad(angleIn));
	float sinIn = Math::sin(Math::deg2rad(angleIn));

	float angleOut = floatRandIncludeHighBound(maxAngle, 180.f);
	CHECK(angleOut > maxAngle);
	float cosOut = Math::cos(Math::deg2rad(angleOut));
	float sinOut = Math::sin(Math::deg2rad(angleOut));

	SUBCASE("Check angle in")
	{
		float targetDirX = cosIn * attackerDirection.x - sinIn * attackerDirection.z;
		float targetDirZ = sinIn * attackerDirection.x + cosIn * attackerDirection.z;
		Vector3 targetDirection{ targetDirX, 0, targetDirZ };

		Vector3 targetPosition = attackerPosition + targetDirection * floatRand();

		CHECK(AttackSystem::CheckAttackAngle(attackerPosition, attackerDirection, targetPosition, maxAngle));
	}

	SUBCASE("Check angle out")
	{
		float targetDirX = cosOut * attackerDirection.x - sinOut * attackerDirection.z;
		float targetDirZ = sinOut * attackerDirection.x + cosOut * attackerDirection.z;
		Vector3 targetDirection{ targetDirX, 0, targetDirZ };

		Vector3 targetPosition = attackerPosition + targetDirection * floatRand();

		CHECK(!AttackSystem::CheckAttackAngle(attackerPosition, attackerDirection, targetPosition, maxAngle));
	}
}