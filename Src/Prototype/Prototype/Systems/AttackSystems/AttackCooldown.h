#pragma once

#include "../../Components/InputComponents.h"
#include "../../Utils/Timer.h"

namespace godot
{
	class AttackCooldown
	{
	protected:
		bool CanAttack(InputComponent input, float attackTime, int64_t& prevHitTimeMillis);
	};
}
