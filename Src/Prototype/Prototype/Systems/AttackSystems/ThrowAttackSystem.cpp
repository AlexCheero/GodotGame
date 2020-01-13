#include "ThrowAttackSystem.h"

#include <OS.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/NodeComponents/ThrowableNodeComponent.h"

void godot::ThrowAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<ThrowableAttackComponent, InputComponent, Spatial*>();
	view.each([&registry, this](entt::entity entity, ThrowableAttackComponent& attackComp, InputComponent input, Spatial* pAttackerSpatial)
	{
		if (!CanAttack(input, attackComp.attackTime, attackComp.prevHitTime))
			return;

		Godot::print("Throw!");

		Node* throwableNode = attackComp.throwableScene->instance();

		pAttackerSpatial->get_tree()->get_root()->add_child(throwableNode);

		//TODO: make global revision and check all such things with assert( != null)
		ThrowableNodeComponent* throwable = Object::cast_to<ThrowableNodeComponent>(throwableNode);
		
		entt::entity throwableEntity = registry.create();
		registry.assign<ThrowableNodeComponent*>(throwableEntity, throwable);

		Transform throwableTransform = throwable->get_transform();
		Transform attackerTransform = pAttackerSpatial->get_transform();
		throwableTransform.origin = attackerTransform.origin;
		throwableTransform.origin += attackerTransform.basis.z;
		throwable->set_transform(throwableTransform);
		throwable->SetCastForce(attackComp.force);
		throwable->apply_central_impulse(attackerTransform.basis.z * attackComp.force);
	});
}