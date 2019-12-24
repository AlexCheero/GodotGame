#include "Throwable.h"

void godot::Throwable::_register_methods()
{
	register_method((char*)"_on_throwable_collide", &Throwable::_on_throwable_collide);
}

void godot::Throwable::_init()
{
	set_contact_monitor(true);
	set_max_contacts_reported(1);
}

void godot::Throwable::_on_throwable_collide(Object* body)
{
	Godot::print("Throwable::_on_throwable_collide");
}
