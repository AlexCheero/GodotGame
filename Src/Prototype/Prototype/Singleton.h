#pragma once

template <typename T>
class Singleton
{
protected:
	static T* instance;

	static void InitInstance(T* inst)
	{
		//TODO: should reinit on scene reset
		//ASSERT(instance == nullptr, "instance already inited");
		ASSERT(inst != nullptr, "trying to init instance with nullptr");

		instance = inst;
	}
public:
	static T* GetInstance() { return instance; }
};