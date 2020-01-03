#include "SimpleFollowView.h"

void godot::SimpleFollowView::_register_methods()
{
	register_property<SimpleFollowView, float>("distance", &SimpleFollowView::distance, 0);
	register_property<SimpleFollowView, float>("x angle", &SimpleFollowView::xAngle, 0);
	register_property<SimpleFollowView, float>("y angle", &SimpleFollowView::yAngle, 0);
}