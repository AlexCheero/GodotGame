#include <core/Godot.hpp>

#include "ECSWorld.h"
#include "Components/Views/EntityView.h"
#include "Components/Views/AttackViews/MeleeAttackView.h"
#include "Components/Views/AttackViews/RangedAttackView.h"
#include "Components/Views/AttackViews/ThrowableAttackView.h"
#include "Components/Views/AttackViews/GrenadeView.h"
#include "Components/Views/HealthView.h"
#include "Components/Views/LocomotionViews/GravityView.h"
#include "Components/Views/LocomotionViews/JumpSpeedView.h"
#include "Components/Views/LocomotionViews/SpeedView.h"
#include "Components/Views/SimpleFollowView.h"
#include "Components/Views/AIViews/PatrolmanView.h"
#include "Components/Views/AIViews/NavMarginView.h"

#include "Nodes/PickableNode.h"
#include "Nodes/ThrowableWeaponNode.h"
#include "Nodes/HTHDamagingArea.h"

using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * o)
{
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * o)
{
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle)
{
	Godot::nativescript_init(handle);
	register_class<ECSWorld>();
	register_class<ThrowableWeaponNode>();
	register_class<EntityView>();
	register_class<MeleeAttackView>();
	register_class<RangedAttackView>();
	register_class<ThrowableAttackView>();
	register_class<HealthView>();
	register_class<GravityView>();
	register_class<JumpSpeedView>();
	register_class<SpeedView>();
	register_class<SimpleFollowView>();
	register_class<PatrolmanView>();
	register_class<NavMarginView>();
	register_class<PickableNode>();
	register_class<HTHDamagingArea>();
	register_class<GrenadeView>();
}