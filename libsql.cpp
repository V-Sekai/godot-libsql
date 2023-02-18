#include "libsql.h"

#include "core/core_bind.h"
#include "core/os/os.h"
#include "editor/project_settings_editor.h"

#include "thirdparty/libsql/sqlite3.h"

Array libsql_fast_parse_row(sqlite3_stmt *stmt) {
	Array result;

	// Get column count
	const int col_count = sqlite3_column_count(stmt);

	// Fetch all column
	for (int i = 0; i < col_count; i++) {
		// Value
		const int col_type = sqlite3_column_type(stmt, i);
		Variant value;

		// Get column value
		switch (col_type) {
			case SQLITE_INTEGER:
				value = Variant(sqlite3_column_int(stmt, i));
				break;

			case SQLITE_FLOAT:
				value = Variant(sqlite3_column_double(stmt, i));
				break;

			case SQLITE_TEXT: {
				int size = sqlite3_column_bytes(stmt, i);
				String str =
						String::utf8((const char *)sqlite3_column_text(stmt, i), size);
				value = Variant(str);
				break;
			}
			case SQLITE_BLOB: {
				PackedByteArray arr;
				int size = sqlite3_column_bytes(stmt, i);
				arr.resize(size);
				memcpy(arr.ptrw(), sqlite3_column_blob(stmt, i), size);
				value = Variant(arr);
				break;
			}
			case SQLITE_NULL: {
				// Nothing to do.
			} break;
			default:
				ERR_PRINT("This kind of data is not yet supported: " + itos(col_type));
				break;
		}

		result.push_back(value);
	}

	return result;
}

LibsqlQuery::LibsqlQuery() {}

LibsqlQuery::~LibsqlQuery() { finalize(); }

void LibsqlQuery::init(Ref<Libsql> p_db, const String &p_query) {
	db = p_db;
	query = p_query;
	stmt = nullptr;
}

bool LibsqlQuery::is_ready() const { return stmt != nullptr; }

String LibsqlQuery::get_last_error_message() const {
	ERR_FAIL_COND_V(db == nullptr, "Database is undefined.");
	return db->get_last_error_message();
}

Variant LibsqlQuery::execute(const Array p_args) {
	if (is_ready() == false) {
		ERR_FAIL_COND_V(prepare() == false, Variant());
	}

	// At this point stmt can't be null.
	CRASH_COND(stmt == nullptr);

	// Error occurred during argument binding
	if (!Libsql::bind_args(stmt, p_args)) {
		ERR_FAIL_V_MSG(Variant(),
				"Error during arguments set: " + get_last_error_message());
	}
	// Execute the query.
	Array result;
	while (true) {
		sqlite3_get_autocommit(db->db);
		const int res = sqlite3_step(stmt);
		if (res == SQLITE_ROW) {
			// Collect the result.
			result.append(libsql_fast_parse_row(stmt));
		} else if (res == SQLITE_DONE) {
			// Nothing more to do.
			break;
		} else {
			// Error
			ERR_BREAK_MSG(true, "There was an error during an SQL execution: " + get_last_error_message());
		}
	}

	if (SQLITE_OK != sqlite3_reset(stmt)) {
		finalize();
		ERR_FAIL_V_MSG(result, "Was not possible to reset the query: " + get_last_error_message());
	}

	return result;
}

Variant LibsqlQuery::batch_execute(Array p_rows) {
	Array res;
	for (int i = 0; i < p_rows.size(); i += 1) {
		ERR_FAIL_COND_V_MSG(p_rows[i].get_type() != Variant::ARRAY, Variant(),
				"An Array of Array is exepected.");
		Variant r = execute(p_rows[i]);
		if (unlikely(r.get_type() == Variant::NIL)) {
			// An error occurred, the error is already logged.
			return Variant();
		}
		res.push_back(r);
	}
	return res;
}

Array LibsqlQuery::get_columns() {
	if (is_ready() == false) {
		ERR_FAIL_COND_V(prepare() == false, Array());
	}

	// At this point stmt can't be null.
	CRASH_COND(stmt == nullptr);

	Array res;
	const int col_count = sqlite3_column_count(stmt);
	res.resize(col_count);

	// Fetch all column
	for (int i = 0; i < col_count; i++) {
		// Key name
		const char *col_name = sqlite3_column_name(stmt, i);
		res[i] = String(col_name);
	}

	return res;
}

bool LibsqlQuery::prepare() {
	ERR_FAIL_COND_V(stmt != nullptr, false);
	ERR_FAIL_COND_V(db == nullptr, false);
	ERR_FAIL_COND_V(query == "", false);

	// Prepare the statement
	int result = sqlite3_prepare_v2(db->get_handler(), query.utf8().ptr(), -1,
			&stmt, nullptr);

	// Cannot prepare query!
	ERR_FAIL_COND_V_MSG(result != SQLITE_OK, false,
			"SQL Error: " + db->get_last_error_message());

	return true;
}

void LibsqlQuery::finalize() {
	if (stmt) {
		sqlite3_finalize(stmt);
		stmt = nullptr;
	}
}

void LibsqlQuery::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_last_error_message"),
			&LibsqlQuery::get_last_error_message);
	ClassDB::bind_method(D_METHOD("execute", "arguments"),
			&LibsqlQuery::execute, DEFVAL(Array()));
	ClassDB::bind_method(D_METHOD("batch_execute", "rows"),
			&LibsqlQuery::batch_execute);
	ClassDB::bind_method(D_METHOD("get_columns"), &LibsqlQuery::get_columns);
}

Libsql::Libsql() {
	db = nullptr;
}
/*
		Open a database file.
		If this is running outside of the editor, databases under res:// are
   assumed to be packed.
		@param path The database resource path.
		@return status
*/
bool Libsql::open(String path) {
	if (!path.strip_edges().length()) {
		return false;
	}
	int result = libsql_open(path.utf8().get_data(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr, nullptr);
	if (result != SQLITE_OK) {
		print_error("Cannot open the database.");
		sqlite3_close_v2(db);
		db = nullptr;
		return false;
	}
	return true;
}

void Libsql::close() {
	// Finalize all queries before close the DB.
	// Reverse order because I need to remove the not available queries.
	for (uint32_t i = queries.size(); i > 0; i -= 1) {
		LibsqlQuery *query =
				Object::cast_to<LibsqlQuery>(queries[i - 1]->get_ref());
		if (query != nullptr) {
			query->finalize();
		} else {
			memdelete(queries[i - 1]);
			queries.remove_at(i - 1);
		}
	}

	if (db) {
		// Cannot close database!
		if (sqlite3_close_v2(db) != SQLITE_OK) {
			print_error("Cannot close database: " + get_last_error_message());
		} else {
			db = nullptr;
		}
	}
}

sqlite3_stmt *Libsql::prepare(const char *query) {
	// Get database pointer
	sqlite3 *dbs = get_handler();

	ERR_FAIL_COND_V_MSG(dbs == nullptr, nullptr,
			"Cannot prepare query! Database is not opened.");

	// Prepare the statement
	sqlite3_stmt *stmt = nullptr;
	int result = sqlite3_prepare_v2(dbs, query, -1, &stmt, nullptr);

	// Cannot prepare query!
	ERR_FAIL_COND_V_MSG(result != SQLITE_OK, nullptr,
			"SQL Error: " + get_last_error_message());
	return stmt;
}

bool Libsql::bind_args(sqlite3_stmt *stmt, Array args) {
	// Check parameter count
	int param_count = sqlite3_bind_parameter_count(stmt);
	if (param_count != args.size()) {
		print_error("LibsqlQuery failed; expected " + itos(param_count) +
				" arguments, got " + itos(args.size()));
		return false;
	}

	/**
	 * libsql data types:
	 * - NULL
	 * - INTEGER (signed, max 8 bytes)
	 * - REAL (stored as a double-precision float)
	 * - TEXT (stored in database encoding of UTF-8, UTF-16BE or UTF-16LE)
	 * - BLOB (1:1 storage)
	 */

	for (int i = 0; i < param_count; i++) {
		int retcode;
		switch (args[i].get_type()) {
			case Variant::Type::NIL:
				retcode = sqlite3_bind_null(stmt, i + 1);
				break;
			case Variant::Type::BOOL:
			case Variant::Type::INT:
				retcode = sqlite3_bind_int(stmt, i + 1, (int)args[i]);
				break;
			case Variant::Type::FLOAT:
				retcode = sqlite3_bind_double(stmt, i + 1, (double)args[i]);
				break;
			case Variant::Type::STRING:
				retcode = sqlite3_bind_text(
						stmt, i + 1, String(args[i]).utf8().get_data(), -1, SQLITE_TRANSIENT);
				break;
			case Variant::Type::PACKED_BYTE_ARRAY:
				retcode =
						sqlite3_bind_blob(stmt, i + 1, PackedByteArray(args[i]).ptr(),
								PackedByteArray(args[i]).size(), SQLITE_TRANSIENT);
				break;
			default:
				print_error(
						"Libsql was passed unhandled Variant with TYPE_* enum " +
						itos(args[i].get_type()) +
						". Please serialize your object into a String or a PoolByteArray.\n");
				return false;
		}

		if (retcode != SQLITE_OK) {
			print_error(
					"LibsqlQuery failed, an error occured while binding argument" +
					itos(i + 1) + " of " + itos(args.size()) + " (libsql errcode " +
					itos(retcode) + ")");
			return false;
		}
	}

	return true;
}

Ref<LibsqlQuery> Libsql::create_query(String p_query) {
	Ref<LibsqlQuery> query;
	query.instantiate();
	query->init(Ref<Libsql>(this), p_query);

	WeakRef *wr = memnew(WeakRef);
	wr->set_obj(query.ptr());
	queries.push_back(wr);

	return query;
}

String Libsql::get_last_error_message() const {
	return sqlite3_errmsg(get_handler());
}

Libsql::~Libsql() {
	// Close database
	close();
	// Make sure to invalidate all associated queries.
	for (uint32_t i = 0; i < queries.size(); i += 1) {
		LibsqlQuery *query =
				Object::cast_to<LibsqlQuery>(queries[i]->get_ref());
		if (query != nullptr) {
			query->init(nullptr, "");
		}
	}
}

void Libsql::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_last_error_message"), &Libsql::get_last_error_message);
	ClassDB::bind_method(D_METHOD("open", "path"), &Libsql::open);
	ClassDB::bind_method(D_METHOD("close"), &Libsql::close);
	ClassDB::bind_method(D_METHOD("create_query", "statement"),
			&Libsql::create_query);
}
