tool
extends EditorProperty
class_name MyDictEditor

var expandBtn = Button.new()
var entry = HBoxContainer.new()

func _ready():
	print("_init")
	expandBtn.text = "Show"
	expandBtn.connect("pressed", self, "_on_expand_pressed")
	
	# set components in the loop
	var componentName = Label.new()
	componentName.align = Label.ALIGN_LEFT
	componentName.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	componentName.text = "Component1"
	
	var compValue = TextEdit.new()
	var value : Dictionary = get_edited_object()[get_edited_property()]
	if (value.has(componentName.text)):
		print("set val: " + value[componentName.text])
		compValue.text = value[componentName.text]
	compValue.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	compValue.connect("text_changed", self, "_on_value_changed", [entry])
	
	entry.add_child(componentName)
	entry.add_child(compValue)
	# --------------------------
	
	add_child(expandBtn)
	set_bottom_editor(expandBtn)
	add_child(entry)
	set_bottom_editor(entry)
	
func _on_value_changed(entry):
	var value : Dictionary = get_edited_object()[get_edited_property()]
	value[entry.get_child(0).text] = entry.get_child(1).text
	emit_changed(get_edited_property(), value)
	
func update_property():
	var value : Dictionary = get_edited_object()[get_edited_property()]
	if (value.has(entry.get_child(0).text)):
		entry.get_child(1).text = value[entry.get_child(0).text]
		print("update property")
	
func _on_expand_pressed():
	if (entry.visible):
		entry.visible = false
		expandBtn.text = "Show"
	else:
		entry.visible = true
		expandBtn.text = "Hide"
