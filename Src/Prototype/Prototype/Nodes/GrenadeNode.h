#pragma once

#include <core/Godot.hpp>
#include <RigidBody.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>

namespace godot
{
	//TODO0: delete this class and make this ecs way
	class GrenadeNode : public RigidBody
	{
		GODOT_CLASS(GrenadeNode, RigidBody)
	private:
		float explosionTime;
		float explosionRadius;
		float damage;
	public:
		static void _register_methods();
		void _init() {}
	};
}