CFLAGS += -O2 -fPIC

build-lib: shim.o
	ar rcs libmvsqlite.a shim.o

build-patched-sqlite3-windows:
	$(CC) -O2 -o sqlite3 \
		-DSQLITE_ENABLE_FTS3 \
		-DSQLITE_ENABLE_FTS4 \
		-DSQLITE_ENABLE_FTS5 \
		-DSQLITE_ENABLE_RTREE \
		-DSQLITE_ENABLE_DBSTAT_VTAB \
		-DSQLITE_ENABLE_MATH_FUNCTIONS \
		-DSQLITE_ENABLE_COLUMN_METADATA \
		-DSQLITE_ENABLE_JSON1 \
		./sqlite3.c ./shim.c ./shell.c \
			-L"$(USERPROFILE)/scoop/apps/openssl-mingw/current/lib64" -L../target/release -lmvsqlite -lpthread -lm -lws2_32 -lbcrypt -luserenv -lntdll

build-library-sqlite3:
	$(CC) -O2 -shared -o libsqlite3.dll \
		-DSQLITE_ENABLE_FTS3 \
		-DSQLITE_ENABLE_FTS4 \
		-DSQLITE_ENABLE_FTS5 \
		-DSQLITE_ENABLE_RTREE \
		-DSQLITE_ENABLE_DBSTAT_VTAB \
		-DSQLITE_ENABLE_MATH_FUNCTIONS \
		-DSQLITE_ENABLE_COLUMN_METADATA \
		-DSQLITE_ENABLE_JSON1 \
		-DMV_STATIC_PATCH \
		./sqlite3.c ./preload.c ./shim.c -L"$(USERPROFILE)/scoop/apps/openssl-mingw/current/lib64" -L../target/release -lmvsqlite -lpthread -lm -lws2_32 -lbcrypt -luserenv -lntdll

build-patched-sqlite3:
	$(CC) -O2 -o sqlite3 \
		-DSQLITE_ENABLE_FTS3 \
		-DSQLITE_ENABLE_FTS4 \
		-DSQLITE_ENABLE_FTS5 \
		-DSQLITE_ENABLE_RTREE \
		-DSQLITE_ENABLE_DBSTAT_VTAB \
		-DSQLITE_ENABLE_MATH_FUNCTIONS \
		-DSQLITE_ENABLE_COLUMN_METADATA \
		-DSQLITE_ENABLE_JSON1 \
		./sqlite3.c ./shim.c ./shell.c \
			-L../target/release -lmvsqlite -lpthread -lm -ldl

clean:
	rm -f libmvsqlite_preload.so preload.o shim.o sqlite3.exe sqlite3

.PHONY: clean
