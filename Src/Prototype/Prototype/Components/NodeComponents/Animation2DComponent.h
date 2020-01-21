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

		static void _register_methods();

		void _init() {}
		void _process(float delta);
	};
}