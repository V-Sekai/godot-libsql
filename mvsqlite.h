#ifndef GD_MVSQLITE_H
#define GD_MVSQLITE_H

#include "core/config/engine.h"
#include "core/object/ref_counted.h"
#include "core/templates/local_vector.h"

// SQLite3
#include "modules/mvsqlite/thirdparty/mvsqlite/mvsqlite-sqlite3/sqlite3.h"
#include "modules/mvsqlite/thirdparty/spmemvfs/spmemvfs.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

extern "C" {
  void init_mvsqlite(void);
  void init_mvsqlite_connection(sqlite3 *db);
  void mvsqlite_autocommit_backoff(sqlite3 *db);
}

typedef int (*sqlite3_initialize_fn)(void);
typedef int (*sqlite3_open_v2_fn)(
    const char *filename,   /* Database filename (UTF-8) */
    sqlite3 **ppDb,            /* OUT: MVSQLite db handle */
    int flags,              /* Flags */
    const char *zVfs        /* Name of VFS module to use */
);
typedef int (*sqlite3_step_fn)(sqlite3_stmt *pStmt);

int sqlite3_step(sqlite3_stmt *pStmt);

int sqlite3_open_v2(
    const char *filename,   /* Database filename (UTF-8) */
    sqlite3 **ppDb,            /* OUT: MVSQLite db handle */
    int flags,              /* Flags */
    const char *zVfs        /* Name of VFS module to use */
);

int sqlite3_open(
    const char *filename,   /* Database filename (UTF-8) */
    sqlite3 **ppDb          /* OUT: MVSQLite db handle */
);

class MVSQLite;

class MVSQLiteQuery : public RefCounted {
  GDCLASS(MVSQLiteQuery, RefCounted);

  MVSQLite *db = nullptr;
  sqlite3_stmt *stmt = nullptr;
  String query;

protected:
  static void _bind_methods();

public:
  MVSQLiteQuery();
  ~MVSQLiteQuery();

  void init(MVSQLite *p_db, const String &p_query);

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

class MVSQLite : public RefCounted {
  GDCLASS(MVSQLite, RefCounted);

  friend class MVSQLiteQuery;

private:
  // sqlite handler
  sqlite3 *db;

  // vfs
  spmemvfs_db_t p_db;
  bool memory_read;

  ::LocalVector<WeakRef *, uint32_t, true> queries;

  sqlite3_stmt *prepare(const char *statement);
  Array fetch_rows(String query, Array args, int result_type = RESULT_BOTH);
  sqlite3 *get_handler() const { return memory_read ? p_db.handle : db; }
  Dictionary parse_row(sqlite3_stmt *stmt, int result_type);

public:
  static bool bind_args(sqlite3_stmt *stmt, Array args);

protected:
  static void _bind_methods();

public:
  enum { RESULT_BOTH = 0, RESULT_NUM, RESULT_ASSOC };

  MVSQLite();
  ~MVSQLite();

  // methods
  bool open(String path);
  bool open_in_memory();
  bool open_buffered(String name, PackedByteArray buffers, int64_t size);
  void close();

  /// Compiles the query into bytecode and returns an handle to it for a faster
  /// execution.
  /// Note: you can create the query at any time, but you can execute it only
  /// when the DB is open.
  Ref<MVSQLiteQuery> create_query(String p_query);

  bool query(String statement);
  bool query_with_args(String statement, Array args);
  Array fetch_array(String statement);
  Array fetch_array_with_args(String statement, Array args);
  Array fetch_assoc(String statement);
  Array fetch_assoc_with_args(String statement, Array args);

  String get_last_error_message() const;
};
#endif
