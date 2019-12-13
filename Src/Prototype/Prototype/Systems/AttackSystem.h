#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	//TODO: rename to PlayerAttackSystem or abstract player input somehow
	//TODO: test or remove testable
	class TESTABLE_CLASS AttackSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
