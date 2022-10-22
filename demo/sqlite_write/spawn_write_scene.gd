extends Node3D


func _ready():
	for i in range(1024):
		var node_3d : Node3D = Node3D.new()
		var script = load("res://sqlite_write/sqlite_write_scene.gd")
		node_3d.set_script(script)
		add_child(node_3d, true)
		node_3d.owner = self

