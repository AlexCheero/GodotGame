#pragma once

template<class T, size_t N>
constexpr size_t GetArraySize(T(&)[N]) { return N; }

#define TAG(Tag) using Tag = entt::tag<#Tag ## _hs>

//needed as tags for type registrator
#define REGISTRABLE_COMPONENT(Component) struct Component
#define REGISTRABLE_TAG(Tag) TAG(Tag)

template <typename T>
struct ComponentMeta {};
#define COMPONENT_META(Type, ...) template<> \
struct ComponentMeta<Type> \
{ \
	constexpr static const char* typeName = #Type; \
	constexpr static const char* properties[] = { __VA_ARGS__ }; \
	constexpr static int propertiesCount = GetArraySize(ComponentMeta<Type>::properties); \
}

template <typename T>
struct TagMeta {};

#define TAG_META(Tag) template<> \
struct TagMeta<Tag> \
{ \
	constexpr static const char* tagName = #Tag; \
	using tagType = Tag; \
}