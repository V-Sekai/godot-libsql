# godot-libsql

libsql wrapper for Godot Engine with wasm text scripts.

Currently renaming to godot-libsql.

## Dependencies

- rust
- https://docs.godotengine.org/en/latest/development/compiling/index.html


## Example usage

- [SQL Queries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/sql_queries.gd)
- [Game Highscore](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/game_highscore.gd)
- [Item Libraries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/item_database.gd)

## Creating wasm programs

https://bindgen.libsql.org/

## Installation for Windows.

```bat
cmd
scoop install llvm-mingw@20220323
scoop install rustup
rustup update
rustup target add x86_64-pc-windows-gnu
rustup target add x86_64-pc-windows-msvc
cd ~/scoop/apps/llvm-mingw/20220906/x86_64-w64-mingw32/lib
ln -s libc++.a libgcc.a # https://www.reddit.com/r/rust/comments/jst1kk/building_rust_without_linking_against_libgcc/
ln -s libunwind.a libgcc_eh.a
```

## Refreshing sqlite3.c

```bat
msys2
pacman -S make clang diff mingw-w64-x86_64-rust
sh configure --enable-all --enable-wasm-runtime
make sqlite3.c
```

## Debugging library loading

```powershell
scoop install Dependencies
```

## Attribution

This engine module for Godot is based on gdsqlite-native by Khairul Hidayat in 2017.

Thanks to the libsql team for their work in 2023.
