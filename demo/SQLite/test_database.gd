extends Node

func _ready() -> void:
	# Create new gdsqlite instance
	var db = SQLite.new();

	# Open item database
	if (!db.open("mvsqlite://test")):
		print("Failed opening database.");
		return;
	var query = "SELECT 
				name
			FROM 
				sqlite_schema
			WHERE 
				type ='table' AND 
				name NOT LIKE 'sqlite_%';"
	var result = db.fetch_array(query)
	print(result)
