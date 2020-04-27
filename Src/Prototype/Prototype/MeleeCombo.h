#pragma once

#include <core/Godot.hpp>

#include <vector>

struct MeleeHit
{
	float damage;
	float attackTime;
	float speed;
	godot::String anim;
	float maxDistance;
	float minDistance;
	//TODO: implement
	//std::vector<Effect> effects;
};

struct ComboComponent
{
	int hitIdx;
	std::vector<MeleeHit> hits;
};