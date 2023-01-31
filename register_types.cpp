#include "register_types.h"

#include "core/object/class_db.h"
#include "mvsqlite.h"

void initialize_libsql_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
	ClassDB::register_class<Libsql>();
	ClassDB::register_class<LibsqlQuery>();
}

void uninitialize_libsql_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
}
