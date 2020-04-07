extends EditorProperty
class_name MyDictEditor

var expandBtn = Button.new()
var entry = HBoxContainer.new()

func _init():
	expandBtn.text = "Show"
	expandBtn.connect("pressed", self, "_on_expand_pressed")
	
	# set components in the loop
	var componentName = Label.new()
	componentName.align = Label.ALIGN_LEFT
	componentName.size_flags_horizontal = Control.SIZE_EXPAND
	componentName.text = "Component1"
	
	var compValue = TextEdit.new()
	compValue.size_flags_horizontal = Control.SIZE_EXPAND
	
	entry.add_child(componentName)
	entry.add_child(compValue)
	# --------------------------
	
	add_child(expandBtn)
	set_bottom_editor(expandBtn)
	add_child(entry)
	set_bottom_editor(entry)
	
#	connect("resized", self, "_on_resized")
	
#set size of components values here
#func _on_resized():
#	entry.get_child(1).rect_min_size.x = entry.rect_size.x / 2
	
func _on_expand_pressed():
	if (entry.visible):
		entry.visible = false
		expandBtn.text = "Show"
	else:
		entry.visible = true
		expandBtn.text = "Hide"
