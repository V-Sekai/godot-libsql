#include "register_types.h"

#include "core/object/class_db.h"
#include "sqlite.h"

void initialize_mvsqlite_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
    return;
  } 
  ClassDB::register_class<SQLite>();
  ClassDB::register_class<SQLiteQuery>();
}

void uninitialize_mvsqlite_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
    return;
  }
}
