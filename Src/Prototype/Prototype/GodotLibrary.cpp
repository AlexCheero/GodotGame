#include <core/Godot.hpp>

#include "ECSWorld.h"
#include "Components/Player.h"
#include "Components/Enemy.h"
#include "Components/Throwable.h"
#include "Components/Views/EntityView.h"
#include "Components/Views/AttackViews/MeleeAttackView.h"
#include "Components/Views/AttackViews/CastAttackView.h"

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
	register_class<Player>();
	register_class<Enemy>();
	register_class<Throwable>();
	register_class<EntityView>();
	register_class<MeleeAttackView>();
	register_class<CastAttackView>();
}