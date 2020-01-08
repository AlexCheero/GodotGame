#include "PatrolmanView.h"

void godot::PatrolmanView::_register_methods()
{
	register_property<PatrolmanView, float>("small view angle", &PatrolmanView::viewAngleSmall, 0);
	register_property<PatrolmanView, float>("big view angle", &PatrolmanView::viewAngleBig, 0);
	register_property<PatrolmanView, float>("long view distance", &PatrolmanView::longViewDistance, 0);
	register_property<PatrolmanView, float>("short view distance", &PatrolmanView::shortViewDistance, 0);
}