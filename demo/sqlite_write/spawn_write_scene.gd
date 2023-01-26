extends Node3D

var create_table : MVSQLiteQuery
var truncate_table : MVSQLiteQuery

func _ready():
	var db : MVSQLite = MVSQLite.new();
	if (!db.open_cluster("mvsqlite")):
		print("Failed opening database.");
		return;
	var create_entity_table : String = """
CREATE TABLE IF NOT EXISTS entity (
	id TEXT PRIMARY KEY NOT NULL CHECK(LENGTH(id) = 36),
	user_data blob NOT NULL CHECK( LENGTH(user_data) = 16) DEFAULT (zeroblob(16)),
	reserved blob NOT NULL CHECK( LENGTH(reserved) = 48)  DEFAULT (zeroblob(48)),
	shard	INTEGER NOT NULL,
	code	INTEGER NOT NULL,
	flags	INTEGER	NOT NULL,
	past_pending	BLOB,
	past_posted BLOB,
	current_pending BLOB,
	current_posted	BLOB,
	timestamp INTEGER NOT NULL
) WITHOUT ROWID, STRICT;
"""
	create_table = db.create_query(create_entity_table)
	print(create_table.execute())
	print(db.get_last_error_message())
	var truncate_entities : String = """DELETE FROM entity;"""
	truncate_table = db.create_query(truncate_entities)
	print(truncate_table.execute())
	print(db.get_last_error_message())
	for i in range(1000):
		var node_3d : Node3D = Node3D.new()
		var script = load("res://sqlite_write/sqlite_write_scene.gd")
		node_3d.set_script(script)
		add_child(node_3d, true)
		node_3d.owner = self

