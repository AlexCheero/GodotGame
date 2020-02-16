#include "GrenadeView.h"

void godot::GrenadeView::_register_methods()
{
	register_property<GrenadeView, float>("explosion time", &GrenadeView::explosionTime, 0);
	register_property<GrenadeView, float>("explosion radius", &GrenadeView::explosionRadius, 0);
	register_property<GrenadeView, float>("damage", &GrenadeView::damage, 0);
}