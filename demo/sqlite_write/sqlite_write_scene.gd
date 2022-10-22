extends Node3D

var db : SQLite = null
var result_create : SQLiteQuery

func _ready():
	db = SQLite.new();
	if (!db.open("test")):
		print("Failed opening database.");
		return;
	var drop : String = """drop table if exists entity;
drop trigger "entity_interpolate_insert";
drop trigger "entity_interpolate_update";
drop trigger "entity_interpolate_delete";
drop view entity_interpolate;
drop view entity_view;
"""
	var create_entity_table : String = """
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
	past_pending	BLOB NOT NULL CHECK( LENGTH(past_pending) <= 64) DEFAULT (zeroblob(64)),
	past_posted BLOB NOT NULL CHECK( LENGTH(past_posted) <= 64) DEFAULT (zeroblob(64)),
	current_pending BLOB NOT NULL CHECK( LENGTH(current_pending) <= 64) DEFAULT (zeroblob(64)),
	current_posted	BLOB NOT NULL CHECK( LENGTH(current_posted) <= 64) DEFAULT (zeroblob(64)),
	timestamp	INTEGER NOT NULL 
) WITHOUT ROWID, STRICT;
"""
#
#CREATE VIEW entity_view
#AS 
#SELECT
# 	"id", "user_data", "reserved", "shard", "code", "flags", "past_pending", "past_posted",
#"current_pending", "current_posted", "timestamp"
#FROM
#	entity;
#
#CREATE VIEW entity_interpolate
#AS 
#SELECT
#	id,
#	user_data,
#	reserved,
#	shard,
#	code,
#	flags,
#	"current_posted" as value
#FROM
#	entity;
#
#CREATE TRIGGER "entity_interpolate_delete"
#	INSTEAD OF DELETE ON "entity_interpolate"
#BEGIN
#DELETE FROM "entity" WHERE ROWID = OLD.ROWID;
#END;
#
#CREATE TRIGGER "entity_interpolate_update"
#	INSTEAD OF UPDATE OF "id", "user_data", "reserved", "shard", "code", "flags", "value"
#	ON "entity_interpolate"
#BEGIN
#UPDATE entity
#   SET "user_data" = old_entity."user_data", "reserved" = old_entity."reserved", "shard" = old_entity."shard", "code" = old_entity."code", "flags" = old_entity."flags", "past_pending" = old_entity.past_pending, "current_pending" = zeroblob(64), "current_posted" = NEW."value", "timestamp" = UNIXEPOCH()
#  FROM (SELECT "id", "user_data", "reserved", "shard", "code", "flags", "past_pending", "past_posted",
#"current_pending", "current_posted", "timestamp" FROM entity) AS old_entity
# WHERE entity.id = NEW.id;
#END;
#
#	var create_trigger = """
#	CREATE TRIGGER "entity_interpolate_insert"
#		INSTEAD OF INSERT ON "entity_interpolate"
#	BEGIN
#	INSERT INTO "entity"
#		("id", "user_data", "reserved", "shard", "code", "flags", "past_pending", "past_posted",
#	"current_pending", "current_posted", "timestamp")
#	VALUES (NEW."id", NEW."user_data", NEW."reserved", NEW."shard", NEW."code", NEW."flags", zeroblob(64), zeroblob(64), zeroblob(64), NEW."value", UNIXEPOCH());
#	END;
#
#	DELETE FROM entity
#	WHERE id = "910bbf73-7b4b-48bd-a3db-b9330f9acc76";"""
	
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
	timestamp=UNIXEPOCH();
"""
	result_create = db.create_query(query_create_original)
	print(result_create.get_columns())

#	var query = """SELECT ("id", "user_data", "reserved", "shard", "code", "flags", "past_pending", "past_posted",
#"current_pending", "current_posted", "timestamp") from entity where "id" = "910bbf73-7b4b-48bd-a3db-b9330f9acc76";"""
#	var result = db.query(query)
#	result = db.fetch_array(query)
#	if result.is_empty():
#		print_debug("Had no results.")
#		return
#	print(result)

func _process(delta):
	if db == null:
		return
	var packed_array : Array
	packed_array.push_back(Transform3D())
	var bytes : PackedByteArray = var_to_bytes(packed_array)
	bytes = bytes.compress(FileAccess.COMPRESSION_ZSTD)
	if bytes.size() > 64:
		return
	bytes.resize(64)
	var statement : Array = ["910bbf73-7b4b-48bd-a3db-b9330f9acc76", bytes]
	var result_batch = result_create.batch_execute([statement])
