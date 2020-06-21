#include "PlayerAttackInputSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>
#include <ConfigFile.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AttackComponents.h"

#include "../../Utils/Utils.h"

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

//TODO0: needed different patterns for mouse
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

//TODO0: probably merge with MeleeAttackCooldownSystem or opposite- split into several systems
void godot::PlayerAttackInputSystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, AttackInputComponent, AttackInputAggregatorComponent, Node*>();
	view.each([&registry, delta](entt::entity entity, AttackInputComponent input, AttackInputAggregatorComponent& inputAggregator, Node* pNode)
	{
		if (input.dir.length_squared() > 0)
		{
			float angle = ClampInputAngle(input.dir);
			for (int i = 0; i < inputAggregator.angles.size(); i++)
			{
				if (inputAggregator.angles[i] < 0)
				{
					if (i > 0 && inputAggregator.angles[i - 1] == angle)
						return;

					inputAggregator.startTime = OS::get_singleton()->get_ticks_msec();
					inputAggregator.angles[i] = angle;
					break;
				}
			}
		}

		if (inputAggregator.angles[0] < 0)
			return;

		//TODO0: don't reset on CD for mouse, without additional input
		if (inputAggregator.angles[inputAggregator.angles.size() - 1] > 0 ||
			OS::get_singleton()->get_ticks_msec() - inputAggregator.startTime >= patternMatchingTime) //TODO: try to reset earlier if out of patterns to match
		{
			int patternIndex = -1;
			int patternLength = 0;
			for (int i = 0; i < MeleeAttackComponent::hitsData.size(); i++)
			{
				const MeleeHit& currentHit = MeleeAttackComponent::hitsData[i];
				//TODO0: don't skip if pattern does not provide alt/leg hit
				if (input.alt != currentHit.alt || input.leg != currentHit.leg)
					continue;

				if (MatchPattern(inputAggregator.angles, currentHit.inputPattern))
				{
					if (currentHit.inputPattern.size() > patternLength)
						patternIndex = i;
				}
			}

			if (patternIndex >= 0)
				registry.emplace<MeleeAttackParameterizedEvent>(entity).hitIndex = patternIndex;

			for (int i = 0; i < inputAggregator.angles.size(); i++)
				inputAggregator.angles[i] = -1;
		}
	});
}