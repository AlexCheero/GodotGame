#pragma once

#include <core/Godot.hpp>
#include <Sprite3D.hpp>


namespace godot
{
	class Animation2DComponent : public Sprite3D
	{
		GODOT_CLASS(Animation2DComponent, Sprite3D)
	private:
		int animCol;
	public:
		int row = 0;

		static void _register_methods()
		{
			register_property<Animation2DComponent, int>("animation column", &Animation2DComponent::animCol, 0);

			register_method((char*)"_process", &Animation2DComponent::_process);
		}

		void _init() {}
		void _process(float delta)
		{
			int frame = animCol + row * 7;
			set_frame(frame);
		}
	};
}