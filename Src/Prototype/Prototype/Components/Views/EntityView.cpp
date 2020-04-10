#include "EntityView.h"

void godot::EntityView::_register_methods()
{
	register_property<EntityView, Dictionary>("components", &EntityView::componentsDict, Dictionary());

	//register_method((char*)"_ready", &EntityView::_ready);
}

/*
void godot::EntityView::_ready()
{
	int childCount = get_child_count();
	for (int i = 0; i < childCount; i++)
	{
		ComponentView* pCompView = Object::cast_to<ComponentView>(get_child(i));
		if (!pCompView)
			continue;
		String key = pCompView->get_name();
		key = key.substr(0, key.length() - 4);//length of "View"
		componentsMap.insert({ key, pCompView });
	}
}
*/