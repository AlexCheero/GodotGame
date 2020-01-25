#pragma once

#include <core/Godot.hpp>
#include <Sprite3D.hpp>


namespace godot
{
	//TODO: rename all node components postfix to Node
	//TODO: add gizmos for direction view in editor
	class Animation2DComponent : public Sprite3D
	{
		GODOT_CLASS(Animation2DComponent, Sprite3D)
	private:
		int animCol;
	public:
		//TODO: move all fields to private and use get/set
		int row;
		bool reflect;
		int numSectors;

		static void _register_methods();

		void _init() {}
		void _process(float delta);
	};
}