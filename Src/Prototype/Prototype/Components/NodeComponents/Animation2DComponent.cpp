#include "Animation2DComponent.h"

void godot::Animation2DComponent::_register_methods()
{
	register_property<Animation2DComponent, int>("animation column", &Animation2DComponent::animCol, 0);
	register_property<Animation2DComponent, bool>("reflect", &Animation2DComponent::reflect, false);
	register_property<Animation2DComponent, int>("num sectors", &Animation2DComponent::numSectors, 0);

	register_method((char*)"_process", &Animation2DComponent::_process);
}

void godot::Animation2DComponent::_process(float delta)
{
	int frame = animCol + row * get_hframes();
	set_frame(frame);
}