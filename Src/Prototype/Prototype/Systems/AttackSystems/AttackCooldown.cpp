#include "AttackCooldown.h"

#include <OS.hpp>

#include "../../Utils/Utils.h"

bool godot::AttackCooldown::CanAttack(InputComponent input, float attackTime, int64_t& prevHitTime)
{
	if (!input.Test(EInput::Attack))
		return false;

	//TODO: move cd reset to base system to have ability to set update frequency
	int64_t currTime = godot::OS::get_singleton()->get_ticks_msec();
	if (prevHitTime + utils::SecondsToMillis(attackTime) > currTime)
		return false;

	prevHitTime = currTime;

	return true;
}