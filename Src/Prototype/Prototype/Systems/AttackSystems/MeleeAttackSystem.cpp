#include "MeleeAttackSystem.h"

#include <OS.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"

//TODO: implement proper hth with blocks and stuff
//TODO: call such systems only by input callback
//TODO: refactor all this mess. try to break into separate systems and spilt between phys and proc
void godot::MeleeAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto animPlayingView = registry.view<AttackAnimPlayingComponent>();
	animPlayingView.each([&registry, delta](entt::entity entity, AttackAnimPlayingComponent& attackPlayingComp)
	{
		attackPlayingComp.playBackTimeLeft -= delta;

		if (attackPlayingComp.playBackTimeLeft <= 0)
			registry.remove<AttackAnimPlayingComponent>(entity);
	});

	auto pileInRemoveOnMoveInputView = registry.view<InputComponent>();
	pileInRemoveOnMoveInputView.each([&registry, delta](entt::entity entity, InputComponent inputComp)
	{
		if (inputComp.moveDir.length_squared() > 0)
			registry.remove_if_exists<PileInTag>(entity);
	});

	auto pileInRemoveOnAnimationEndView = registry.view<PileInTag>(entt::exclude<AttackAnimPlayingComponent>);
	registry.remove<PileInTag>(pileInRemoveOnAnimationEndView.begin(), pileInRemoveOnAnimationEndView.end());

	auto incrementComboView = registry.view<IncrementComboTag, MeleeAttackComponent>();
	incrementComboView.less([](MeleeAttackComponent& attackComp)
	{
		attackComp.hitIdx++;
		if (attackComp.hitIdx > attackComp.hits.size() - 1)
			attackComp.hitIdx = 0;
	});
	registry.remove<IncrementComboTag>(incrementComboView.begin(), incrementComboView.end());
}