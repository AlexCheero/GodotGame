#pragma once

#include <core/Godot.hpp>
#include <Area.hpp>

#include "entt/entt.hpp"

#include "../Views/EntityView.h"

namespace godot
{
	enum class EPickableType
	{
		None = -1,
		MeleeWeapon,
		RangedWeapon,
		ThrowableWeapon,
		Medkit,
		Buff,
		Key,

		Last
	};

	class PickableNodeComponent : public Area
	{
		GODOT_CLASS(PickableNodeComponent, Area)
	private:
		void SetType(int i);
		int GetType() { return static_cast<int>(type); }
		EPickableType type;

		entt::entity pickerEntity;

		void _on_Pickable_body_entered(Node* pNode);
	public:
		static void _register_methods();
		void _init();
		void _ready();
	};
}
