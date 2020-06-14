#pragma once

//TODO: implement separate NodeSingleton with auto init on _init/_ready and auto delete on _exit_tree
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

	static void DeleteInstance() { instance = nullptr; }
public:
	static T* GetInstance() { return instance; }
};