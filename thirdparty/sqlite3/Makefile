CFLAGS += -O2 -fPIC

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
	CFLAGS += -lws2_32 -lbcrypt -luserenv -lntdll
else
    CFLAGS += -lpthread -lm -ldl
endif

build-patched-sqlite3:
	cd ../mvsqlite && cargo build --release -p mvsqlite
	gcc -O2 -o sqlite3 \
		-DSQLITE_ENABLE_FTS3 \
		-DSQLITE_ENABLE_FTS4 \
		-DSQLITE_ENABLE_FTS5 \
		-DSQLITE_ENABLE_RTREE \
		-DSQLITE_ENABLE_DBSTAT_VTAB \
		-DSQLITE_ENABLE_MATH_FUNCTIONS \
		-DSQLITE_ENABLE_COLUMN_METADATA \
		-DSQLITE_ENABLE_JSON1 \
		./sqlite3.c ./shim.c ./shell/shell.c \
			-L"$(USERPROFILE)/scoop/apps/openssl-mingw/current/lib64" -I../sqlite3 -L../mvsqlite/target/release -lmvsqlite -lpthread -lm $(CFLAGS)

build-lib: shim.o
	ar rcs libmvsqlite.a shim.o

build-library-sqlite3:
	gcc -O2 -shared -o libsqlite3.dll \
		-DSQLITE_ENABLE_FTS3 \
		-DSQLITE_ENABLE_FTS4 \
		-DSQLITE_ENABLE_FTS5 \
		-DSQLITE_ENABLE_RTREE \
		-DSQLITE_ENABLE_DBSTAT_VTAB \
		-DSQLITE_ENABLE_MATH_FUNCTIONS \
		-DSQLITE_ENABLE_COLUMN_METADATA \
		-DSQLITE_ENABLE_JSON1 \
		-DMV_STATIC_PATCH \
		./sqlite3.c ./shim.c -L"$(USERPROFILE)/scoop/apps/openssl-mingw/current/lib64" -L../mvsqlite/target/release -lmvsqlite -lpthread -lm -lws2_32 -lbcrypt -luserenv -lntdll

clean:
	rm -f shim.o sqlite3.exe sqlite3

.PHONY: clean
