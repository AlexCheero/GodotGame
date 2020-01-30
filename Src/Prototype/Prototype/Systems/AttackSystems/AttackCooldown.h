#pragma once

#include "../../Components/InputComponents.h"
#include "../../Utils/Timer.h"

namespace godot
{
	class AttackCooldown
	{
	private:
		//TODO1: use or remove
		utils::Timer timer;
	protected:
		bool CanAttack(InputComponent input, float attackTime, int64_t& prevHitTime);
	};
}
