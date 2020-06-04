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

template<typename T, std::size_t... I>
T godot::EntityView::ConvertComponentFromDict_impl(const Dictionary& dict, std::index_sequence<I...>)
{
	return T{ dict[ComponentMeta<T>::properties[I]]... };
}

template<typename T>
T godot::EntityView::ConvertComponentFromDict(const Dictionary& dict)
{
	return ConvertComponentFromDict_impl<T>(dict, std::make_index_sequence<ComponentMeta<T>::propertiesCount>{});
}

#ifdef DEBUG
template<typename T>
bool godot::EntityView::CheckKeys(Dictionary& dict)
{
	if (dict.size() != ComponentMeta<T>::propertiesCount)
		return false;

	for (int i = 0; i < ComponentMeta<T>::propertiesCount; i++)
	{
		if (!dict.has(ComponentMeta<T>::properties[i]))
			return false;
	}

	return true;
}
#endif

void godot::EntityView::_register_methods()
{
	register_property<EntityView, Dictionary>("components", &EntityView::componentsDict, Dictionary());
	register_property<EntityView, Array>("tags", &EntityView::tags, Array());
}

template<typename T>
bool godot::EntityView::ConvertToComponent(T& comp)
{
	String componentKey(ComponentMeta<T>::typeName);
	//assert that component's type name always ends with "Component"
	ASSERT(componentKey.find_last("Component") == componentKey.length() - 9, "component's type name doesn't ends with \"Component\"");
	componentKey = componentKey.substr(0, componentKey.length() - 9);
	if (!componentsDict.has(componentKey))
		return false;

	Dictionary dict = componentsDict[componentKey];

#ifdef DEBUG
	ASSERT(CheckKeys<T>(dict), "wrong keys in components dict of entity view");
#endif

	comp = ConvertComponentFromDict<T>(dict);

	return true;
}

void godot::EntityView::ConstructComponents(entt::registry& registry, entt::entity entity)
{
	ConstructComponents_impl<REGISTERED_COMPONENTS>(registry, entity);
}

void godot::EntityView::ConstructTags(entt::registry& registry, entt::entity entity)
{
	ConstructTags_impl<REGISTERED_TAGS>(registry, entity);
}