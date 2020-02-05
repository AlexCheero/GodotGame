#include "PlayerVelocitySystem.h"

#include <Input.hpp>
#include <AnimationTree.hpp>

#include "../../Components/InputComponents.h"

//TODO: move anim code to different system
#include "../../Components/SimpleComponents.h"
#include "../../Utils/Utils.h"
#include "core/math/math_funcs.h"
//----------------------------------------

void godot::PlayerVelocitySystem::Update(VelocityComponent& velocityComp, SpeedComponent speedComp, Basis camBasis, Vector2 input)
{
	Vector3 flatVelocity = Vector3(0, 0, 0);
	flatVelocity += camBasis.x * input.x;
	flatVelocity -= camBasis.z * input.y;

	flatVelocity.normalize();
	flatVelocity *= speedComp.speed;

	velocityComp.velocity.x = flatVelocity.x;
	velocityComp.velocity.z = flatVelocity.z;
}

void godot::PlayerVelocitySystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<PlayerTag>, VelocityComponent, InputComponent, SpeedComponent, Camera*, RotationDirectionComponent, AnimationTree*>();
	view.less([&registry](entt::entity entity, VelocityComponent& velocityComp, InputComponent input
		, SpeedComponent speedComp, Camera* pCam,/*anim params starts from here*/ RotationDirectionComponent rotationComp, AnimationTree* pAnimTree)
	{
		Update(velocityComp, speedComp, pCam->get_transform().get_basis(), input.moveDir);

		//TODO: move anim code to different system
		bool right = velocityComp.velocity.cross(rotationComp.direction).y < 0;
		//TODO: angle isn't precise cause velocity's y
		//float angle = Math::acos(velocityComp.velocity.normalized().dot(rotationComp.direction));
		//if (right)
		//	angle *= -1;
		
		//float x = Math::cos(angle) * input.moveDir.x - Math::sin(angle) * input.moveDir.y;
		//float y = Math::sin(angle) * input.moveDir.x + Math::cos(angle) * input.moveDir.y;
		//Vector2 blend_pos = Vector2{ x, y }.normalized();
		//pAnimTree->set("parameters/BlendSpace2D/blend_position", blend_pos);

		float angle = Math::rad2deg(Math::acos(velocityComp.velocity.normalized().dot(rotationComp.direction)));
		if (right)
		{
			angle = 360 - angle;
			//angle *= -1;
		}

		//TODO: diagonal animations are wrong, probably because of blending
		Vector2 blend_pos;
		//no movement
		if (input.moveDir.length_squared() == 0)
			blend_pos = Vector2{ 0, 0 };
		//fwd + rght
		else if (angle > 45 - 22.5f && angle <= 45 + 22.5f)
			blend_pos = Vector2{ 1, -1 }.normalized();
		//rght
		else if (angle > 90 - 22.5f && angle <= 90 + 22.5f)
			blend_pos = Vector2{ 1, 0 };
		//bwd + rght
		else if (angle > 135 - 22.5f && angle <= 135 + 22.5f)
			blend_pos = Vector2{ 1, 1 }.normalized();
		//bwd
		else if (angle > 180 - 22.5f && angle <= 180 + 22.5f)
			blend_pos = Vector2{ 0, 1 }.normalized();
		//bwd + lft
		else if (angle > 225 - 22.5f && angle <= 225 + 22.5f)
			blend_pos = Vector2{ -1, 1 }.normalized();
		//lft
		else if (angle > 270 - 22.5f && angle <= 270 + 22.5f)
			blend_pos = Vector2{ -1, 0 }.normalized();
		//fwd + lft
		else if (angle > 315 - 22.5f && angle <= 315 + 22.5f)
			blend_pos = Vector2{ -1, -1 }.normalized();
		//fwd
		else if (angle > 315 + 22.5f || angle <= 0 + 22.5f)
			blend_pos = Vector2{ 0, -1 };

		pAnimTree->set("parameters/BlendSpace2D/blend_position", blend_pos);
		//----------------------------------------
	});
}