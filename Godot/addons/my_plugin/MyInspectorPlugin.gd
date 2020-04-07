tool
extends EditorInspectorPlugin

enum Components {
	Comp1,
	Comp2,
	Comp3,
	Comp4,
	Comp5,
	Comp6,
	Comp7,
	Comp8,
	Comp9,
	Comp10,
	Comp11,
	Comp12,
	Comp13,
	Comp14,
	Comp15,
	Comp16,
	Comp17,
	Comp18,
	Comp19,
	Comp20,
	Comp21,
	Comp22,
	Comp23,
	Comp24,
	Comp25,
	Comp26,
	Comp27,
	Comp28,
	Comp29,
	Comp30,
	Comp31,
	Comp32,
	Comp33,
	Comp34,
	Comp35,
	Comp36,
	Comp37,
	Comp38,
	Comp39,
	Comp40
}

var addingComponentMenu: PopupMenu

func can_handle(object):
	return object is TestScript

func parse_begin(object):
	var btn = Button.new()
	btn.text = "add component"
	btn.connect("pressed", self, "_on_add_component_pressed")
	add_custom_control(btn)
	
	addingComponentMenu = PopupMenu.new()
#	addingComponentMenu.connect("id_pressed", self, "_on_id_pressed")
	addingComponentMenu.connect("index_pressed", self, "_on_index_pressed", [object])
	
	for component in Components:
		addingComponentMenu.add_item(component)
	
#	var submenu = PopupMenu.new()
#	submenu.add_item("submenu item")
#	submenu.name = "submenu"
#	addingComponentMenu.add_child(submenu)
#	addingComponentMenu.add_submenu_item("label", "submenu")
#	submenu.allow_search = true
	
	addingComponentMenu.allow_search = true
	add_custom_control(addingComponentMenu)

func _on_add_component_pressed():
	# fix focus on menu switching off and switching off menu on clicking outside
	var winWidth = OS.window_size.x
	var mouseX = addingComponentMenu.get_global_mouse_position().x
	var menuWidth = addingComponentMenu.rect_size.x
	
	var menuRectPosition = addingComponentMenu.get_global_mouse_position()
	if (menuRectPosition.x + menuWidth > winWidth):
		menuRectPosition.x = winWidth - menuWidth
	
	addingComponentMenu.rect_position = menuRectPosition
	addingComponentMenu.visible = !addingComponentMenu.visible
	
#func _on_id_pressed(id):
#	print("_on_id_pressed: " + String(id))
	
func _on_index_pressed(index, object):
	if (object.components.has(index)):
		print("delete")
		object.components.erase(index)
		return
		
	if (index == 0):
		object.components[Components.Comp1] = Vector3(1, 1, 1)
		
	object.add_child(Node.new())
#	print("_on_index_pressed: " + addingComponentMenu.get_item_text(index) + ", for: " + object.name)

func parse_property(object, type, path, hint, hint_text, usage):
	if type == TYPE_DICTIONARY:
		add_property_editor(path, MyDictEditor.new())
		return true
	return false
