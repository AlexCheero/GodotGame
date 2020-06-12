#include "PlayerAttackInputSystem.h"

#include <Spatial.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/InputComponents.h"

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

void godot::PlayerAttackInputSystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<PlayerTag, AttackInputComponent, AttackInputAggregatorComponent, Node*>();
	view.each([](AttackInputComponent input, AttackInputAggregatorComponent& inputAggregator, Node* pNode)
	{
		float angle = ClampInputAngle(input.dir);
		for (auto& pattern : attackPatterns)
		{
			for (int i = 0; i < inputAggregator.attacks.size(); i++)
			{
				
			}
		}

		return;

		//if (input.dir.length_squared() > 0)
		//	Godot::print("input x: " + String::num(input.dir.x) + ", y: " + String::num(input.dir.y));

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

		auto& dirs = inputAggregator.dirs;

		bool prevDirHold = false;
		//TODO0: also add reset by CD and by finished move
		if (input.dir.length_squared() == 0)
		{
			//Reset aggregator
			for (size_t i = 0; i < inputAggregator.dirs.size(); i++)
				dirs[i] = Vector2(0, 0);
		}
		else
		{
			for (size_t i = 0; i < inputAggregator.dirs.size(); i++)
			{
				//TODO0: remake with approx eq
				if (dirs[i].x == input.dir.x && dirs[i].y == input.dir.y)
				{
					prevDirHold = true;
					break;
				}

				if (dirs[i].length_squared() == 0)
				{
					dirs[i] = input.dir;
					Godot::print(String::num_int64(i) + ". save dir " + ", x: " + String::num(dirs[i].x) + ", y: " + String::num(dirs[i].y));
					break;
				}
			}
		}

		//TODO0: remake with approx eq
		//TODO0: read from config
		bool jab = dirs[0].x == 0 && dirs[0].y > 0;
		//bool rightHook = dirs[0].x > 0 && dirs[0].y == 0 && dirs[1].x > 0 && dirs[1].y > 0;
		bool rightHook = dirs[0].x > 0 && dirs[0].y > 0 && dirs[1].x == 0 && dirs[1].y > 0;
		bool leftHook = dirs[0].x < 0 && dirs[0].y == 0 && dirs[1].x < 0 && dirs[1].y > 0;

		//implement pattern matching and on matched cutout

		bool inputSchemeMatch = jab || rightHook || leftHook;

		if (!prevDirHold)
		{
			if (jab)
				Godot::print("jab!");
			if (rightHook)
				Godot::print("right hook!");
			if (leftHook)
				Godot::print("left hook!");
		}
	});
}