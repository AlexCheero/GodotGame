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

		Vector3 bZ = rotationComp.direction;
		Vector3 bY = utils::globalY;
		Vector3 bX = bZ.cross(bY).normalized();

		Basis b(bX, bY, bZ);
		b.orthonormalize();
		Vector3 flatVel = velocityComp.velocity;
		flatVel.y = 0;
		flatVel.normalize();
		Vector3 xform = b.xform(flatVel).normalized();

		Vector2 animVec{ xform.x, xform.z };

		//TODO: diagonal anims looks a bit weird because of blending problem
		pAnimTree->set("parameters/BlendSpace2D/blend_position", animVec);
		//----------------------------------------
	});
}