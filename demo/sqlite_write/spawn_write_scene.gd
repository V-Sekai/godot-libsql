extends Node3D


func _ready():
	var db : SQLite = SQLite.new();
	if (!db.open("test")):
		print("Failed opening database.");
		return;
	
	var truncate_entities : String = """
DELETE FROM entity;
	"""
	db.query(truncate_entities)
	for i in range(128):
		var node_3d : Node3D = Node3D.new()
		var script = load("res://sqlite_write/sqlite_write_scene.gd")
		node_3d.set_script(script)
		add_child(node_3d, true)
		node_3d.owner = self

