#pragma once

template<class T, size_t N>
constexpr size_t GetArraySize(T(&)[N]) { return N; }

template <typename T>
struct ComponentMeta {};
//TODO: separate component name from "Component" to set only its actual name in editor
//TODO: rename to ComponentSMeta
//should add type into EntityView::ConstructComponents_impl types list TODO: try to make it automatically
#define REGISTER_COMPONENT(Type, ...) template<> \
struct ComponentMeta<Type> \
{ \
	constexpr static const char* typeName = #Type; \
	constexpr static const char* properties[] = { __VA_ARGS__ }; \
	constexpr static int propertiesCount = GetArraySize(ComponentMeta<Type>::properties); \
}