#include "PlayerAttackInputSystem.h"

#include <Spatial.hpp>

#include "../../Components/SimpleComponents.h"

#include "../../Utils/Utils.h"

//TODO0: read from config
std::vector<float> angles = { 0, 45, 90, 135, 180, 225, 270, 315, 360 };
//TODO0: needed different patterns and CD for mouse
std::vector<float> jabPattern = { 90 };
std::vector<float> rightHookPattern = { 45, 90 };
std::vector<float> leftHookPattern = { 135, 90 };

std::vector<std::vector<float>> attackPatterns = { jabPattern, rightHookPattern, leftHookPattern };

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

void godot::PlayerAttackInputSystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, AttackInputComponent, AttackInputAggregatorComponent, Node*>();
	view.each([](AttackInputComponent input, AttackInputAggregatorComponent& inputAggregator, Node* pNode)
	{
		//TODO0: cant use patterns that goes through the center if using zero length check
		if (input.dir.length_squared() == 0 || inputAggregator.angles[inputAggregator.angles.size() - 1] > 0)
		{
			for (int i = 0; i < attackPatterns.size(); i++)
			{
				//TODO0: choose longest pattern if more than one matched
				if (MatchPattern(inputAggregator.angles, attackPatterns[i]))
				{
					Godot::print(String::num_int64(i) + " pattern matched");
				}
			}

			//TODO0: also add reset by CD
			for (int i = 0; i < inputAggregator.angles.size(); i++)
				inputAggregator.angles[i] = -1;
			return;
		}

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

		return;

		//====DEBUG INDICATOR STUFF====
		/*
		Spatial* indicator = Object::cast_to<Spatial>(pNode->get_node("StickIndicator"));

		Vector3 newOrigin(indicator->get_transform().origin);
		newOrigin.x = -input.dir.x;//x is inverted some why
		newOrigin.z = input.dir.y;

		Transform newTransform(indicator->get_transform());
		newTransform.set_origin(newOrigin);

		indicator->set_transform(newTransform);
		*/
		//====DEBUG INDICATOR STUFF====
	});
}