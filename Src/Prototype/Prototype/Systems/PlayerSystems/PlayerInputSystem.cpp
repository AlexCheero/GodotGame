#include "PlayerInputSystem.h"

#include <Input.hpp>

#include "../../Components/InputComponents.h"
#include "../../Utils/Utils.h"

namespace //private
{
	template<typename T>
	void ProcessInputKey(entt::registry& registry, bool pressed)
	{
		if (pressed)
		{
			auto view = registry.view<PlayerTag>(entt::exclude<T>);
			registry.assign<T>(view.begin(), view.end());
		}
		else
		{
			auto view = registry.view<PlayerTag, T>();
			registry.remove<T>(view.begin(), view.end());
		}
	}

	godot::Vector2 GetInputDirection(godot::Input* pInput, const char* actionPrefix)
	{
		float horizontal = pInput->get_action_strength(actionPrefix + godot::String("_right")) - pInput->get_action_strength(actionPrefix + godot::String("_left"));
		float vertical = pInput->get_action_strength(actionPrefix + godot::String("_up")) - pInput->get_action_strength(actionPrefix + godot::String("_down"));

		return { horizontal, vertical };
	}

	template<typename Tag, typename Val>
	void ProcessInputAxis(entt::registry& registry, godot::Vector2 newVec)
	{
		auto unchangedView = registry.view <PlayerTag, Val>(entt::exclude<Tag>);
		unchangedView.less([&registry, newVec](entt::entity entity, Val& comp)
		{
			if (utils::Vector2Equals(comp.dir, newVec))
				return;
			
			comp.dir = newVec;
			registry.assign<Tag>(entity);
		});

		auto changedView = registry.view <PlayerTag, Tag, Val>();
		changedView.less([&registry, newVec](entt::entity entity, Val& comp)
		{
			if (utils::Vector2Equals(comp.dir, newVec))
				registry.remove<Tag>(entity);
		});
	}
}

void godot::PlayerInputSystem::HandleInput(entt::registry& registry)
{
	godot::Input* pInput = godot::Input::get_singleton();

	ProcessInputKey<AttackPressedTag>(registry, pInput->is_action_pressed("attack"));
	ProcessInputKey<JumpPressedTag>(registry, pInput->is_action_pressed("jump"));
	ProcessInputKey<ChooseMeleePressedTag>(registry, pInput->is_action_pressed("choose_melee"));
	ProcessInputKey<ChooseRangedPressedTag>(registry, pInput->is_action_pressed("choose_ranged"));
	ProcessInputKey<ChooseThrowablePressedTag>(registry, pInput->is_action_pressed("choose_throwable"));

	ProcessInputAxis<GET_AXIS_TYPES(Rotation)>(registry, GetInputDirection(pInput, "ui"));
	ProcessInputAxis<GET_AXIS_TYPES(MoveDir)>(registry, GetInputDirection(pInput, "move"));
}