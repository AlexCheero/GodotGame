#pragma once

#include <stdint.h>

#include "../Utils.h"

struct AttackComponent
{
	float distance;
	float damage;
	float angle;
	float attackTime;
	int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
};
