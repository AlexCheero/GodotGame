#pragma once

template<class T, size_t N>
constexpr size_t GetArraySize(T(&)[N]) { return N; }

template <typename T>
struct ComponentsMeta {};
//should add type into EntityView::ConstructComponents_impl types list
#define REGISTER_COMPONENT(Type, ...) template<> \
struct ComponentsMeta<Type> \
{ \
	constexpr static const char* typeName = #Type; \
	constexpr static const char* properties[] = { __VA_ARGS__ }; \
	constexpr static int propertiesCount = GetArraySize(ComponentsMeta<Type>::properties); \
}