#include "HTHDamagingArea.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"

void godot::HTHDamagingArea::_register_methods()
{
	register_property<HTHDamagingArea, float>("damage", &HTHDamagingArea::damage, 0);

	register_method((char*)"_on_Area_body_entered", &HTHDamagingArea::_on_Area_body_entered);
}

void godot::HTHDamagingArea::_on_Area_body_entered(EntityHolderNode* pEntityHolder)
{
	if (!pEntityHolder)
		return;

	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();

	registry.get<HealthComponent>(pEntityHolder->GetEntity()).hp -= damage;
}