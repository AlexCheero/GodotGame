#pragma once

#include <Input.hpp>

#include "entt/entt.hpp"

#ifdef DEBUG
#define TESTABLE_CLASS __declspec(dllexport) 
#else
#define TESTABLE_CLASS
#endif // DEBUG

namespace godot
{
	class BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) = 0;
	};
}
