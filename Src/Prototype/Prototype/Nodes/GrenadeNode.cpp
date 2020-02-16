#include "GrenadeNode.h"

#include <OS.hpp>
#include <SceneTree.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../Utils/Utils.h"

#include "EntityHolderNode.h"


void godot::GrenadeNode::_register_methods()
{
	register_property<GrenadeNode, float>("explosion time", &GrenadeNode::explosionTime, 0);
	register_property<GrenadeNode, float>("explosion radius", &GrenadeNode::explosionRadius, 0);
	register_property<GrenadeNode, float>("damage", &GrenadeNode::damage, 0);
}