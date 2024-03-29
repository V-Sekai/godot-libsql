/**************************************************************************/
/*  libsql.h                                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef LIBSQL_H
#define LIBSQL_H

#include "core/object/ref_counted.h"
#include "core/templates/local_vector.h"

#include "thirdparty/libsql/sqlite3.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

class Libsql;

class LibsqlQuery : public RefCounted {
	GDCLASS(LibsqlQuery, RefCounted);

	Ref<Libsql> db;
	sqlite3_stmt *stmt = nullptr;
	String query;

protected:
	static void _bind_methods();

public:
	LibsqlQuery();
	~LibsqlQuery();

	void init(Ref<Libsql> p_db, const String &p_query);

	bool is_ready() const;

	/// Returns the last error message.
	String get_last_error_message() const;

	/// Executes the query.
	/// ```
	/// var query = db.create_query("SELECT * FROM table_name;")
	/// print(query.execute())
	/// # prints: [[0, 1], [1, 1], [2, 1]]
	/// ```
	///
	/// You can also pass some arguments:
	/// ```
	/// var query = db.create_query("INSERT INTO table_name (column1, column2)
	/// VALUES (?, ?); ") print(query.execute([0,1])) # prints: []
	/// ```
	///
	/// In case of error, a Variant() is returned and the error is logged.
	/// You can also use `get_last_error_message()` to retrieve the message.
	Variant execute(Array p_args = Array());

	/// Expects an array of arguments array.
	/// Executes N times the query, for N array.
	/// ```
	/// var query = db.create_query("INSERT INTO table_name (column1, column2)
	/// VALUES (?, ?); ") query.batch_execute([[0,1], [1,2], [2,3]])
	/// ```
	/// The above script insert 3 rows.
	///
	/// Also works with a select:
	/// ```
	/// var query = db.create_query("SELECT * FROM table_name WHERE column1 = ?;")
	/// query.batch_execute([[0], [1], [2]])
	/// ```
	/// Returns: `[[0,1], [1,2], [2,3]]`
	Variant batch_execute(Array p_rows);

	/// Return the list of columns of this query.
	Array get_columns();

	void finalize();

private:
	bool prepare();
};

class Libsql : public RefCounted {
	GDCLASS(Libsql, RefCounted);

	friend class LibsqlQuery;

private:
	// sqlite handler
	sqlite3 *db;

	::LocalVector<WeakRef *, uint32_t, true> queries;

	sqlite3_stmt *prepare(const char *statement);
	Array fetch_rows(String query, Array args, int result_type = RESULT_BOTH);
	sqlite3 *get_handler() const { return db; }
	Dictionary parse_row(sqlite3_stmt *stmt, int result_type);

public:
	static bool bind_args(sqlite3_stmt *stmt, Array args);

protected:
	static void _bind_methods();

public:
	enum {
		RESULT_BOTH = 0,
		RESULT_NUM,
		RESULT_ASSOC
	};

	Libsql();
	~Libsql();

	// methods
	bool open(String path);
	bool open_cluster(String path);

	void close();

	/// Compiles the query into bytecode and returns an handle to it for a faster
	/// execution.
	/// Note: you can create the query at any time, but you can execute it only
	/// when the DB is open.
	Ref<LibsqlQuery> create_query(String p_query);
	String get_last_error_message() const;
};
#endif // LIBSQL_H
