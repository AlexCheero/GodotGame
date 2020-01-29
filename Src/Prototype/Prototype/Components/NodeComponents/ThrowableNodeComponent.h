#pragma once

#include <core/Godot.hpp>
#include <RigidBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	class ThrowableNodeComponent : public RigidBody
	{
		GODOT_CLASS(ThrowableNodeComponent, RigidBody)
	private:
		float damagePerForce;

		entt::entity hittedEntity;
		float throwForce;
	public:
		entt::entity GetHittedEntity() const { return hittedEntity; }
		void SetThrowForce(float force) { throwForce = force; }
		float GetDamage() { return damagePerForce * throwForce; }

		static void _register_methods();
		void _init();
		void _ready();
		void _on_throwable_collide(Node* pNode);
	};
}