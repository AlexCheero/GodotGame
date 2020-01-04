#pragma once

#include "../../Components/InputComponents.h"

namespace godot
{
	class AttackCooldown
	{
	protected:
		bool CanAttack(InputComponent input, float attackTime, int64_t& prevHitTime);
	};
}
