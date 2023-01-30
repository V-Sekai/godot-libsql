extends Node3D

var create_table : LibsqlQuery
var truncate_table : LibsqlQuery

func _ready():
	var db : Libsql = Libsql.new();
	if (!db.open("libsql")):
		print("Failed opening database.");
		return;
	# https://github.com/eliovir/rust-examples/blob/master/fibonacci.rs
	var query = """
CREATE FUNCTION fibonacci LANGUAGE wasm AS '(module
  (type (;0;) (func (param i64) (result i64)))
  (func (;0;) (type 0) (param i64) (result i64)
	(local i32 i64 i64)
	block  ;; label = @1
	  block  ;; label = @2
		block  ;; label = @3
		  local.get 0
		  i32.wrap_i64
		  local.tee 1
		  i32.const 0
		  i32.lt_s
		  br_if 0 (;@3;)
		  i64.const 1
		  local.set 2
		  local.get 1
		  br_table 0 (;@3;) 2 (;@1;) 1 (;@2;)
		end
		unreachable
	  end
	  local.get 1
	  i32.const -1
	  i32.add
	  local.set 1
	  i64.const 1
	  local.set 0
	  loop  ;; label = @2
		local.get 0
		local.get 3
		i64.add
		local.set 2
		local.get 0
		local.set 3
		local.get 2
		local.set 0
		local.get 1
		i32.const -1
		i32.add
		local.tee 1
		br_if 0 (;@2;)
	  end
	end
	local.get 2)
  (memory (;0;) 16)
  (global (;0;) i32 (i32.const 1048576))
  (global (;1;) i32 (i32.const 1048576))
  (export "memory" (memory 0))
  (export "fibonacci" (func 0))
  (export "__data_end" (global 0))
  (export "__heap_base" (global 1)))
'
	"""
	var result : LibsqlQuery = db.create_query(query)
	print(result.execute())
	result = db.create_query("SELECT fibonacci(36)")
	print(result.execute())
