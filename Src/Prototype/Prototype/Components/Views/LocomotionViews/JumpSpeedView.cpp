#include "JumpSpeedView.h"

void godot::JumpSpeedView::_register_methods()
{
	register_property<JumpSpeedView, float>("jump speed", &JumpSpeedView::speed, 0);
}