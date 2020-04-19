#pragma once

template<class T, size_t N>
constexpr size_t GetArraSize(T(&)[N]) { return N; }

template <typename T>
struct ComponentMeta {};
//TODO: separate component name from "Component" to set only its actual name in editor
#define REGISTER_COMPONENT(Type, ...) template<> \
struct ComponentMeta<Type> \
{ \
	constexpr static const char* name = #Type; \
	constexpr static const char* properties[] = { __VA_ARGS__ }; \
	constexpr static int propertiesCount = GetArraSize(ComponentMeta<Type>::properties); \
}