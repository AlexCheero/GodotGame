#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class ThrowableWeaponSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}