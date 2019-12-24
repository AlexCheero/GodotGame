#pragma once

#include <core/Godot.hpp>
#include <RigidBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	class Throwable : public RigidBody
	{
		GODOT_CLASS(Throwable, RigidBody)
	private:
		entt::entity hittedEntity;
		float damagePerForce;
		float castForce;
	public:
		entt::entity GetHittedEntity() const { return hittedEntity; }
		void SetCastForce(float force) { castForce = force; }
		float GetDamage() { return damagePerForce * castForce; }

		static void _register_methods();
		void _init();
		void _on_throwable_collide(Node* pNode);
	};
}