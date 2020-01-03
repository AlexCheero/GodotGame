#include "CastAttackView.h"

void godot::CastAttackView::_register_methods()
{
	register_property<CastAttackView, float>("distance", &CastAttackView::distance, 0);
	register_property<CastAttackView, float>("damage", &CastAttackView::damage, 0);
	register_property<CastAttackView, float>("attackTime", &CastAttackView::attackTime, 0);
}
