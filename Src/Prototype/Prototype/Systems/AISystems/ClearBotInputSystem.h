#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class ClearBotInputSystem : public BaseSystem
	{
	private:
		template<typename Type, typename... Types>
		void ClearInput(entt::registry& registry);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}