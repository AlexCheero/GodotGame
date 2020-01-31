#pragma once

#include <core/Godot.hpp>
#include <RigidBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	class ThrowableWeaponNode : public RigidBody
	{
		GODOT_CLASS(ThrowableWeaponNode, RigidBody)
	private:
		float damagePerForce;
		float throwForce;
	public:
		void SetThrowForce(float force) { throwForce = force; }
		float GetDamage() { return damagePerForce * throwForce; }

		static void _register_methods();
		void _init();
		void _ready();
		void _on_throwable_collide(Node* pNode);
	};
}