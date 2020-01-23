#pragma once

#include <core/Godot.hpp>
#include <Sprite3D.hpp>


namespace godot
{
	//TODO: rename all node components postfix to Node
	class Animation2DComponent : public Sprite3D
	{
		GODOT_CLASS(Animation2DComponent, Sprite3D)
	private:
		int animCol;
	public:
		int row;
		bool reflect;
		int numSectors;

		static void _register_methods();

		void _init() {}
		void _process(float delta);
	};
}