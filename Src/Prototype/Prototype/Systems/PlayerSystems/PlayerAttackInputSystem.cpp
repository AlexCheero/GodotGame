#include "PlayerAttackInputSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>
#include <ConfigFile.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AttackComponents.h"

#include "../../Utils/Utils.h"

std::unordered_map<godot::String, std::vector<float>> godot::PlayerAttackInputSystem::patterns;

//TODO0: read from config
//TODO0: needed different patterns and CD for mouse
std::vector<float> angles = { 0, 45, 90, 135, 180, 225, 270, 315, 360 };
constexpr int64_t patternMatchingTime = 400;

float godot::PlayerAttackInputSystem::ClampInputAngle(Vector2 dir)
{
	float angle = utils::Rad2deg(dir.angle());
	if (angle < 0)
		angle += 360;
	float resultAngle = angles[0];

	float lowestDelta = 360;
	for (int i = 0; i < angles.size(); i++)
	{
		float newDelta = abs(angle - angles[i]);
		if (newDelta < lowestDelta)
		{
			resultAngle = angles[i];
			lowestDelta = newDelta;
		}
	}

	if (resultAngle == 360)
		resultAngle = 0;

	return resultAngle;
}

bool godot::PlayerAttackInputSystem::MatchPattern(AttackInputAggregatorComponent::AggregatorType aggregation, std::vector<float> pattern)
{
	int i = 0;
	int patternIndex = 0;
	for (; i < aggregation.size() - (pattern.size() - patternIndex - 1); i++)
	{
		if (aggregation[i] == pattern[patternIndex])
			patternIndex++;
		else
			patternIndex = 0;

		if (patternIndex == pattern.size())
			return true;
	}

	return false;
}

void godot::PlayerAttackInputSystem::Init()
{
	ConfigFile* cfg = ConfigFile::_new();
	Error err = cfg->load("res://Configs/hit_patterns.cfg");
	ASSERT(err == Error::OK, "cannot load hit_patterns config");
	PoolStringArray sections = cfg->get_sections();
	for (int i = 0; i < sections.size(); i++)
	{
		String section = sections[i];
		Array arr = cfg->get_value(section, "pattern");

		patterns.insert({ section, {} });
		for (int i = 0; i < arr.size(); i++)
			patterns[section].push_back(arr[i]);
	}

	cfg->free();

	for (const auto& pair : patterns)
	{
		Godot::print(String(pair.first) + ":");
		for (auto val : patterns[pair.first])
			Godot::print("    " + String::num(val));
	}
}

void godot::PlayerAttackInputSystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, AttackInputComponent, AttackInputAggregatorComponent, Node*>();
	view.each([delta](AttackInputComponent input, AttackInputAggregatorComponent& inputAggregator, Node* pNode)
	{
		//TODO?: reset on every input
		if (inputAggregator.angles[0] == -1)
			inputAggregator.startTime = OS::get_singleton()->get_ticks_msec();

		if (input.dir.length_squared() > 0)
		{
			float angle = ClampInputAngle(input.dir);
			for (int i = 0; i < inputAggregator.angles.size(); i++)
			{
				if (inputAggregator.angles[i] < 0)
				{
					if (i > 0 && inputAggregator.angles[i - 1] == angle)
						return;

					inputAggregator.angles[i] = angle;
					break;
				}
			}
		}

		if (inputAggregator.angles[inputAggregator.angles.size() - 1] > 0 ||
			OS::get_singleton()->get_ticks_msec() - inputAggregator.startTime >= patternMatchingTime) //TODO0: reset earlier if out of patterns to match
		{
			int patternIndex = -1;
			int patternLength = 0;
			for (int i = 0; i < MeleeAttackComponent::hitsData.size(); i++)
			{
				auto& pattern = MeleeAttackComponent::hitsData[i].inputPattern;
				if (MatchPattern(inputAggregator.angles, pattern))
				{
					if (pattern.size() > patternLength)
						patternIndex = i;
				}
			}

			if (patternIndex >= 0)
				Godot::print(MeleeAttackComponent::hitsData[patternIndex].name + " matched");

			for (int i = 0; i < inputAggregator.angles.size(); i++)
				inputAggregator.angles[i] = -1;
		}
	});
}