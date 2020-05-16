#include "ClearBotInputSystem.h"

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"

template<typename Type, typename... Types>
void godot::ClearBotInputSystem::ClearInput(entt::registry& registry)
{
	auto view = registry.view<BotTag, Type>();
	registry.remove<Type>(view.begin(), view.end());
	if constexpr (sizeof...(Types))
		ClearInput<Types...>(registry);
}

void godot::ClearBotInputSystem::operator()(float delta, entt::registry& registry)
{
	ClearInput
		<
		    JumpPressedTag,
		    AttackPressedTag,
		    ChooseMeleePressedTag,
		    ChooseRangedPressedTag,
		    ChooseThrowablePressedTag
		>
		(registry);
}