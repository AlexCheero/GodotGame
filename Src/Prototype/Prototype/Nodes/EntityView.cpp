#include "EntityView.h"

#include "../Components/RegisteredTypes.h"

template<typename Type, typename... Types>
void godot::EntityView::ConstructComponents_impl(entt::registry& registry, entt::entity entity)
{
	Type component{};
	if (ConvertToComponent(component))
		registry.emplace<Type>(entity, component);
	if constexpr (sizeof...(Types))
		ConstructComponents_impl<Types...>(registry, entity);
}

template<typename Tag>
bool godot::EntityView::HasTag()
{
	for (int i = 0; i < tags.size(); i++)
	{
#ifdef DEBUG
		//assert that tag's name always ends with "Tag"
		String debugTagName(TagMeta<Tag>::tagName);
		ASSERT(debugTagName.find_last("Tag") == debugTagName.length() - 3, "tag's name doesn't ends with \"Tag\"");
#endif
		String tagName = String(tags[i]) + "Tag";
		if (tagName == TagMeta<Tag>::tagName)
			return true;
	}

	return false;
}

template<typename Tag, typename ...Tags>
void godot::EntityView::ConstructTags_impl(entt::registry& registry, entt::entity entity)
{
	if (HasTag<Tag>())
		registry.emplace<Tag>(entity);
	if constexpr (sizeof...(Tags))
		ConstructTags_impl<Tags...>(registry, entity);
}

void godot::EntityView::_register_methods()
{
	register_property<EntityView, Dictionary>("components", &EntityView::componentsDict, Dictionary());
	register_property<EntityView, Array>("tags", &EntityView::tags, Array());
}

void godot::EntityView::ConstructComponents(entt::registry& registry, entt::entity entity)
{
	ConstructComponents_impl
		<
			MeleeAttackComponent,
			RangedAttackComponent,
			ThrowableAttackComponent,
			GrenadeAttackComponent,
			HealthComponent,
			GravityComponent,
			JumpSpeedComponent,
			SpeedComponent,
			PatrolmanComponent,
			NavMarginComponent,
			MeleeWeaponComponent,
			DecisionMakingComponent
		>
		(registry, entity);
}

void godot::EntityView::ConstructTags(entt::registry& registry, entt::entity entity)
{
	ConstructTags_impl<REGISTERED_TAGS>(registry, entity);
}