#pragma once

template <typename T>
class Singleton
{
protected:
	static T* instance;

	static void InitInstance(T* inst)
	{
		ASSERT(instance == nullptr, "instance already inited");
		ASSERT(inst != nullptr, "trying to init instance with nullptr");

		instance = inst;
	}

	static void ResetInstance() { instance = nullptr; }
public:
	static T* GetInstance() { return instance; }
};