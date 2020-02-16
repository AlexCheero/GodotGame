#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	//TODO: this class should not inherit KinematicBody
	class EntityHolderNode : public KinematicBody
	{
		GODOT_CLASS(EntityHolderNode, KinematicBody)
	private:
		entt::entity entity = entt::null;
	public:
		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		static void _register_methods() {}
		void _init() {}
	};
}