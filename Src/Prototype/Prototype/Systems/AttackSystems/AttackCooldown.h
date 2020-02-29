#pragma once

#include "../../Components/InputComponents.h"
#include "../../Utils/Timer.h"

namespace godot
{
	class AttackCooldown
	{
	protected:
		//TODO: change to CDExpired and move input check outside
		bool CanAttack(InputComponent input, float attackTime, int64_t& prevHitTimeMillis);
	};
}
