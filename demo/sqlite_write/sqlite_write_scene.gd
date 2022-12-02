extends Node3D

var db : MVSQLite = null
var result_create : MVSQLiteQuery
var result_delete : MVSQLiteQuery
var uuid : String 

func _ready():
	db = MVSQLite.new();
	if (!db.open("mvsqlite")):
		print("Failed opening database.");
		return;
	var select_uuid : String = """
	SELECT lower(hex(randomblob(4)) || '-' || hex(randomblob(2)) || '-' || '4' || 
	substr(hex( randomblob(2)), 2) || '-' || 
	substr('AB89', 1 + (abs(random()) % 4) , 1)  ||
	substr(hex(randomblob(2)), 2) || '-' ||
	hex(randomblob(6))) as uuid;
	"""
	uuid = db.fetch_array(select_uuid)[0]["uuid"]
	var query_create_original = """
INSERT INTO entity ("id", "user_data", "reserved", "shard", "code", "flags", "past_pending", "past_posted",
"current_pending", "current_posted", "timestamp")
VALUES (?, zeroblob(16), zeroblob(48), 0, 0, 0, zeroblob(64), zeroblob(64), zeroblob(64), ?, UNIXEPOCH());
"""
	result_create = db.create_query(query_create_original)
	var query_delete = """
	DELETE FROM entity
	WHERE id = ?;
"""
	result_delete = db.create_query(query_delete)
	
func _process(_delta):
	if db == null:
		return
	var packed_array : Array = Array()
	packed_array.push_back(global_transform)
	var bytes : PackedByteArray = var_to_bytes(packed_array)
	bytes = bytes.compress(FileAccess.COMPRESSION_ZSTD)
	var statement : Array = [uuid, bytes]
	var _result_batch = result_create.batch_execute([statement])

func _exit_tree():
	var statement : Array = [uuid]
	var _result_batch = result_delete.batch_execute([statement])
