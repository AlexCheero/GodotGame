#include "SpeedView.h"

void godot::SpeedView::_register_methods()
{
	register_property<SpeedView, float>("speed", &SpeedView::speed, 0);
}