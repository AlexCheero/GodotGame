#pragma once

#include "entt/entt.hpp"

#ifdef DEBUG
#define TESTABLE_CLASS __declspec(dllexport) 
#else
#define TESTABLE_CLASS
#endif // DEBUG

namespace godot
{
	//TODO: implement custom update frequency
	class BaseSystem
	{
	public:
		virtual ~BaseSystem() {};
		virtual void operator()(float delta, entt::registry& registry) = 0;
	};
}
