#pragma once

#include <core/Godot.hpp>
#include <Area.hpp>
#include <KinematicBody.hpp>

#include "entt/entt.hpp"

#include "../Nodes/EntityView.h"

namespace godot
{
	enum class EPickableType
	{
		None = -1,
		MeleeWeapon,
		RangedWeapon,
		ThrowableWeapon,
		Grenade,
		Medkit,
		Buff,
		Key,

		Last
	};

	class PickableNode : public Area
	{
		GODOT_CLASS(PickableNode, Area)
	private:
		void SetType(int i);
		int GetType() { return static_cast<int>(type); }
		EPickableType type;

		void _on_Pickable_body_entered(KinematicBody* pBody);
	public:
		static void _register_methods();
		void _init() {}
	};
}
