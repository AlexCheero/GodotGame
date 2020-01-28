#include "RangedAttackView.h"

void godot::RangedAttackView::_register_methods()
{
	register_property<RangedAttackView, int>("ammo count", &RangedAttackView::ammoCount, 0);
	register_property<RangedAttackView, float>("distance", &RangedAttackView::distance, 0);
	register_property<RangedAttackView, float>("damage", &RangedAttackView::damage, 0);
	register_property<RangedAttackView, float>("attackTime", &RangedAttackView::attackTime, 0);
}
