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

//TODO: problem with matching patterns, that starts from diagonal angles, on arrow keys
bool godot::PlayerAttackInputSystem::MatchPattern(const AttackInputAggregatorComponent::AggregatorType& aggregation, const std::vector<float>& pattern)
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

//TODO0: convert once on config load
//TODO0: convert mouse input to stick angles because mouse patterns can have more angles than stcik patterns
std::vector<float> godot::PlayerAttackInputSystem::ConvertToMousePattern(const std::vector<float>& pattern)
{
	ASSERT(pattern.size() > 0, "trying to convert empty pattern");
	if (pattern.size() == 1)
		return pattern;

	std::vector<float> convertedpattern;
	convertedpattern.push_back(pattern[0]);
	for (int i = 1; i < pattern.size(); i++)
	{
		Vector2 prevDir = GetDirFromAngle(pattern[i - 1]);
		Vector2 currDir = GetDirFromAngle(pattern[i]);
		
		float convertedAngle = ClampInputAngle(currDir - prevDir);
		convertedpattern.push_back(convertedAngle);
	}

	return convertedpattern;
}

//TODO: this conversion fails if there will be more than 8 sectors!!!
godot::Vector2 godot::PlayerAttackInputSystem::GetDirFromAngle(float angle)
{
	float radAngle = utils::Deg2rad(angle);
	return Vector2(utils::sign(cos(radAngle)), utils::sign(sin(radAngle)));
}

//TODO0: probably merge with MeleeAttackCooldownSystem or opposite- split into several systems
void godot::PlayerAttackInputSystem::Tick(float delta, entt::registry& registry)
{
	auto aggregateView = registry.view<PlayerTag, AttackInputComponent, AttackInputAggregatorComponent>();
	aggregateView.each([&registry](entt::entity entity, AttackInputComponent& input, AttackInputAggregatorComponent& inputAggregator)
	{
		if (input.dir.length_squared() == 0)
			return;
		
		float angle = ClampInputAngle(input.dir);

		//reset input for mouse
		input.dir = Vector2(0, 0);

		//TODO0: try to refactor it in more ecs/dod way. move up into separate system or something
		if (registry.has<AttackAnimPlayingComponent>(entity))
			return;

		if (inputAggregator.angles[0] < 0)
		{
			inputAggregator.startTime = OS::get_singleton()->get_ticks_msec();
			inputAggregator.angles[0] = angle;
		}
		else
		{
			for (int i = 1; i < inputAggregator.angles.size(); i++)
			{
				if (inputAggregator.angles[i] > -1)
					continue;

				if (utils::RealEquals(inputAggregator.angles[i - 1], angle, 0.1f))
					break;

				inputAggregator.startTime = OS::get_singleton()->get_ticks_msec();
				inputAggregator.angles[i] = angle;

				break;
			}
		}
	});

	auto matchingView = registry.view<PlayerTag, AttackInputComponent, AttackInputAggregatorComponent>();
	matchingView.each([&registry](entt::entity entity, AttackInputComponent input, AttackInputAggregatorComponent& inputAggregator)
	{
		if (inputAggregator.angles[0] < 0)
			return;

		bool cdExpired = OS::get_singleton()->get_ticks_msec() - inputAggregator.startTime >= patternMatchingTime;
		if (inputAggregator.angles[inputAggregator.angles.size() - 1] < 0 && !cdExpired) //TODO: try to reset earlier if out of patterns to match
			return;
		
		int patternIndex = -1;
		int patternLength = 0;
		for (int i = 0; i < MeleeAttackComponent::hitsData.size(); i++)
		{
			const MeleeHit& currentHit = MeleeAttackComponent::hitsData[i];
			//TODO0: don't skip if pattern does not provide alt/leg hit
			if (input.alt != currentHit.alt || input.leg != currentHit.leg)
				continue;

			const std::vector<float>& pattern = !input.mouseInput ? currentHit.inputPattern : ConvertToMousePattern(currentHit.inputPattern);
			if (MatchPattern(inputAggregator.angles, pattern))
			{
				if (currentHit.inputPattern.size() > patternLength)
					patternIndex = i;
			}
		}

		if (patternIndex >= 0)
			registry.emplace<MeleeAttackParameterizedEvent>(entity).hitIndex = patternIndex;

		for (int i = 0; i < inputAggregator.angles.size(); i++)
			inputAggregator.angles[i] = -1;
	});
}