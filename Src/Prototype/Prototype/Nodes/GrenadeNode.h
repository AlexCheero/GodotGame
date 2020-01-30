#pragma once

#include <core/Godot.hpp>
#include <RigidBody.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>

namespace godot
{
	class GrenadeNode : public RigidBody
	{
		GODOT_CLASS(GrenadeNode, RigidBody)
	private:
		float explosionTime;
		float explosionRadius;
		float damage;

		Ref<PhysicsShapeQueryParameters> params;
		Ref<SphereShape> attackShape;
		float startTime;
		Array hitted;
	public:
		Array GetHitted() { return hitted; }
		float GetDamage() { return damage; }

		static void _register_methods();
		void _init() {}
		void _ready();
		void _process(float delta);
	};
}