#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	class EnemyNodeComponent : public KinematicBody
	{
		GODOT_CLASS(EnemyNodeComponent, KinematicBody)
	private:
		entt::entity entity;
	public:
		//TODO: probably entity get/set should be moved to base EntityViewComponent class when it's done
		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		static void _register_methods() {}
		void _init() {}
	};
}