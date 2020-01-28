#include "ThrowableAttackView.h"

void godot::ThrowableAttackView::_register_methods()
{
	register_property<ThrowableAttackView, int>("ammo count", &ThrowableAttackView::ammoCount, 0);
	register_property<ThrowableAttackView, Ref<PackedScene>>("throwable", &ThrowableAttackView::throwableScene, Ref<PackedScene>(nullptr));
	register_property<ThrowableAttackView, float>("force", &ThrowableAttackView::force, 0);
	register_property<ThrowableAttackView, float>("attackTime", &ThrowableAttackView::attackTime, 0);
}