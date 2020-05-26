#pragma once

template<class T, size_t N>
constexpr size_t GetArraySize(T(&)[N]) { return N; }

template <typename T>
struct ComponentsMeta {};
//TODO: try to implement type registartor as a separate project/application
//should add type into EntityView::ConstructComponents_impl types list
#define REGISTER_COMPONENT(Type, ...) template<> \
struct ComponentsMeta<Type> \
{ \
	constexpr static const char* typeName = #Type; \
	constexpr static const char* properties[] = { __VA_ARGS__ }; \
	constexpr static int propertiesCount = GetArraySize(ComponentsMeta<Type>::properties); \
}

#define DECLARE_TAG(Tag) using Tag = entt::tag<#Tag ## _hs>

template <typename T>
struct TagMeta {};

//should add type into EntityView::ConstructTags_impl types list
#define DECLARE_REGISTERED_TAG(Tag) DECLARE_TAG(Tag); \
template<> \
struct TagMeta<Tag> \
{ \
	constexpr static const char* tagName = #Tag; \
	using tagType = Tag; \
}