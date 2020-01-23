#include "BillboardRotationSystem.h"

#include <Sprite3D.hpp>
#include <Spatial.hpp>

#include <math.h> 

//TODO: wrap all engine code in utils to easily remove it in future
#include "core/math/math_funcs.h"

#include "../Components/SimpleComponents.h"
#include "../Components/NodeComponents/Animation2DComponent.h"

//TODO: probably implement it via shader
//TODO: test for 8 directional sprite, like imp
void godot::BillboardRotationSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity mainCamEntity = registry.view<entt::tag<MainCameraTag> >()[0];
	Transform mainCamTransform = registry.get<Spatial*>(mainCamEntity)->get_global_transform();
	Vector3 camFwd = -mainCamTransform.get_basis().z;

	//TODO: implement not for only animated
	auto view = registry.view<Animation2DComponent*, Spatial*>();
	view.each([mainCamTransform, camFwd](Animation2DComponent* pSprite, Spatial* pSpatial) mutable
	{
		Vector3 spatialFwd = pSpatial->get_global_transform().get_basis().z;
		Vector3 spatialLeft = pSpatial->get_global_transform().get_basis().x;

		//TODO: should I use local normal?
		//using global up as normal, so spatialFwd and spatialLeft should be always orthogonal to it
		const Plane xzPlane(Vector3{ 0, 1, 0 }, 0);
		camFwd = xzPlane.project(camFwd);

		camFwd.normalize();
		spatialFwd.normalize();
		spatialFwd.normalize();

		float fwdDot = spatialFwd.dot(camFwd);
		float leftDot = spatialLeft.dot(camFwd);

		float angle = Math::rad2deg(Math::acos(fwdDot));

		//TODO: move to view and implement symmetrical sprites
		//const bool reflect = false;

		bool rightSide = leftDot > 0;
		if (rightSide)
			angle = 360 - angle;
		angle = fmod(angle, 360);

		//TODO: assert(angle >= 0 && angle <= 360)
		//TODO: move to view
		const int numSectors = 4;
		float anglePerSprite = 360 / numSectors;
		int sector = angle / anglePerSprite;
		if (fmod(angle, anglePerSprite) > anglePerSprite / 2)
			sector++;
		sector %= 4;

		pSprite->row = sector;
	});
}