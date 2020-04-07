tool
extends EditorPlugin

var plugin: EditorInspectorPlugin

func _enter_tree():
	 # EditorInspectorPlugin is a resource, so we use `new()` instead of `instance()`.
	plugin = preload("res://addons/my_plugin/MyInspectorPlugin.gd").new()
	add_inspector_plugin(plugin)


func _exit_tree():
	remove_inspector_plugin(plugin)
