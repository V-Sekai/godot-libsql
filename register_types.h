#ifndef LIBSQL_REGISTER_TYPES_H
#define LIBSQL_REGISTER_TYPES_H

#include "modules/register_module_types.h"

void initialize_libsql_module(ModuleInitializationLevel p_level);
void uninitialize_libsql_module(ModuleInitializationLevel p_level);

#endif // LIBSQL_REGISTER_TYPES_H
