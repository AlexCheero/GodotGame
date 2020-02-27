#pragma once

#include "BaseSystem.h"

namespace godot
{
	template<typename System, typename... Systems>
	class SystemChain : public BaseSystem
	{
	private:
		using SystemsVec = std::vector<std::unique_ptr<BaseSystem>>;
		SystemsVec m_systems;
	public:
		SystemChain()
		{
			Emplace<System, Systems...>();
		}

		template<typename System, typename... Systems>
		void Emplace()
		{
			Append(new System);
			if constexpr (sizeof...(Systems))
				Emplace<Systems...>();
		}

		void Append(BaseSystem* system)
		{
			m_systems.push_back(std::unique_ptr<BaseSystem>(system));
		}

		virtual void operator()(float delta, entt::registry& registry) override
		{
			for (auto& system : m_systems)
				(*system)(delta, registry);
		}
	};
}
