#include "MeleeAttackView.h"

void godot::MeleeAttackView::_register_methods()
{
	register_property<MeleeAttackView, float>("distance", &MeleeAttackView::distance, 0);
	register_property<MeleeAttackView, float>("damage", &MeleeAttackView::damage, 0);
	register_property<MeleeAttackView, float>("angle", &MeleeAttackView::angle, 0);
	register_property<MeleeAttackView, float>("attackTime", &MeleeAttackView::attackTime, 0);
	register_property<MeleeAttackView, String>("collision layer", &MeleeAttackView::collisionLayerName, "");
}