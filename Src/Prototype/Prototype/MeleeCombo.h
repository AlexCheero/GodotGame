#pragma once

#include <core/Godot.hpp>

#include <vector>

struct MeleeHit
{
	float damage;
	float attackTime;
	godot::String anim;
	float maxDistance;
	float minDistance;
	//TODO: implement
	//std::vector<Effect> effects;
};

struct ComboComponent
{
	std::vector<MeleeHit> hits;
	int hitIdx = 0;
	int64_t prevHitTimeMillis = -utils::SecondsToMillis(hits[0].attackTime);
	//TODO: remove hardcode
	static constexpr int64_t maxComboIntervalMillis = 3000;

	MeleeHit GetCurrentHit() { return hits[hitIdx]; }
};