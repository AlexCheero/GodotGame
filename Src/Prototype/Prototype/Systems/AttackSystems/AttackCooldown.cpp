#include "AttackCooldown.h"

#include <OS.hpp>

#include "../../Utils/Utils.h"

bool godot::AttackCooldown::CanAttack(InputComponent input, float attackTime, int64_t& prevHitTimeMillis)
{
	if (!input.Test(EInput::Attack))
		return false;

	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	if (prevHitTimeMillis + utils::SecondsToMillis(attackTime) > currTimeMillis)
		return false;

	prevHitTimeMillis = currTimeMillis;

	return true;
}