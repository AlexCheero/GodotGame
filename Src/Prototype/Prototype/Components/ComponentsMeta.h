#pragma once

template <typename T>
struct ComponentMeta
{
	constexpr static const char* name;
	constexpr static int propertiesCount;
};

#define REGISTER_COMPONENT(Type, propsCnt) template<> \
struct ComponentMeta<Type> \
{ \
	constexpr static const char* name = #Type; \
	constexpr static int propertiesCount = propsCnt; \
}