#pragma once

#include <core/Godot.hpp>
#include <Area.hpp>

#include "entt/entt.hpp"

#include "../Components/Views/EntityView.h"
#include "EntityHolderNode.h"

namespace godot
{
	class HTHDamagingArea : public Area
	{
		GODOT_CLASS(HTHDamagingArea, Area)
	private:
		entt::entity ownerEntity;
		float damage;

		void _on_Area_body_entered(EntityHolderNode* pEntityHolder);
		void _assign_owner_entity();
	public:
		static void _register_methods();
		void _init() {}
		void _ready();
	};
}
