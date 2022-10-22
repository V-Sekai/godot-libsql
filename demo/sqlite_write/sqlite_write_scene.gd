extends Node3D

var db : SQLite = null
var result_create : SQLiteQuery
var result_delete : SQLiteQuery
var uuid : String 

func _ready():
	var crypto = Crypto.new()
	uuid = crypto.generate_random_bytes(4).hex_encode().trim_prefix("0b") + \
	'-' + crypto.generate_random_bytes(2).hex_encode().trim_prefix("0b") + \
	'-' + '4' + crypto.generate_random_bytes(2).hex_encode().trim_prefix("0b").substr(0, 3) + \
	'-' + "AB89".substr((abs(randi()) % 4), 1) + crypto.generate_random_bytes(2).hex_encode().trim_prefix("0b").substr(0, 3) + \
	'-' + crypto.generate_random_bytes(6).hex_encode().trim_prefix("0b")
	uuid = uuid.to_lower()
	db = SQLite.new();
	if (!db.open("test")):
		print("Failed opening database.");
		return;
	var _drop : String = """drop table if exists entity;
drop trigger "entity_interpolate_insert";
drop trigger "entity_interpolate_update";
drop trigger "entity_interpolate_delete";
drop view entity_interpolate;
drop view entity_view;
"""
	
	var select_uuid : String = """
	SELECT lower(hex(randomblob(4)) || '-' || hex(randomblob(2)) || '-' || '4' || 
	substr(hex( randomblob(2)), 2) || '-' || 
	substr('AB89', 1 + (abs(random()) % 4) , 1)  ||
	substr(hex(randomblob(2)), 2) || '-' ||
	hex(randomblob(6))) as uuid;
	"""
	uuid = db.fetch_array(select_uuid)[0]["uuid"]
	var _create_entity_table : String = """
DROP TABLE IF EXISTS entity;
CREATE TABLE entity (
	id TEXT PRIMARY KEY NOT NULL CHECK(LENGTH(id) = 36) DEFAULT(lower(
	hex(randomblob(4)) || '-' || hex(randomblob(2)) || '-' || '4' || 
	substr(hex( randomblob(2)), 2) || '-' || 
	substr('AB89', 1 + (abs(random()) % 4) , 1)  ||
	substr(hex(randomblob(2)), 2) || '-' || 
	hex(randomblob(6))
  )),
	user_data blob NOT NULL CHECK( LENGTH(user_data) = 16) DEFAULT (zeroblob(16)),
	reserved blob NOT NULL CHECK( LENGTH(reserved) = 48)  DEFAULT (zeroblob(48)),
	shard	INTEGER NOT NULL,	
	code	INTEGER NOT NULL,	
	flags	INTEGER	NOT NULL,
	past_pending	BLOB NOT NULL CHECK( LENGTH(past_pending) <= 1024) DEFAULT (zeroblob(64)),
	past_posted BLOB NOT NULL CHECK( LENGTH(past_posted) <= 1024) DEFAULT (zeroblob(64)),
	current_pending BLOB NOT NULL CHECK( LENGTH(current_pending) <= 1024) DEFAULT (zeroblob(64)),
	current_posted	BLOB NOT NULL CHECK( LENGTH(current_posted) <= 1024) DEFAULT (zeroblob(64)),
	timestamp INTEGER NOT NULL
) WITHOUT ROWID, STRICT;
"""	
	var query_create_original = """
INSERT INTO entity ("id", "user_data", "reserved", "shard", "code", "flags", "past_pending", "past_posted",
"current_pending", "current_posted", "timestamp")
VALUES (?, zeroblob(16), zeroblob(48), 0, 0, 0, zeroblob(64), zeroblob(64), zeroblob(64), ?, UNIXEPOCH())
ON CONFLICT(id) DO UPDATE SET
	user_data=excluded.user_data,
	reserved=excluded.reserved,
	shard=excluded.shard,
	code=excluded.code,
	flags=excluded.flags,
	past_pending=excluded.current_pending,
	past_posted=excluded.current_posted,
	current_pending=zeroblob(64),
	current_posted=zeroblob(64),
	timestamp=UNIXEPOCH()
WHERE timestamp < UNIXEPOCH();
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
	packed_array.push_back(Transform3D())
	var bytes : PackedByteArray = var_to_bytes(packed_array)
	bytes = bytes.compress(FileAccess.COMPRESSION_ZSTD)
	var statement : Array = [uuid, bytes]
	var _result_batch = result_create.batch_execute([statement])

func _exit_tree():
	var statement : Array = [uuid]
	var _result_batch = result_delete.batch_execute([statement])
