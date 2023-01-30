# godot-mvsqlite

[mvsqlite](https://github.com/losfair/mvsqlite) wrapper for Godot Engine

## Dependencies

* rust
* https://docs.godotengine.org/en/latest/development/compiling/index.html

## Git Installation

Download using git.

```
git clone https://github.com/V-Sekai/godot-mvsqlite mvsqlite
```

Move to the modules folder inside of the Godot Engine source.

```
mv mvsqlite godot/modules/
```

## Example usage

- [SQL Queries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/sql_queries.gd)
- [Game Highscore](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/game_highscore.gd)
- [Item Libraries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/item_database.gd)

# mvsqlite for Windows

```bat
REM for windows
REM scoop install llvm-mingw@20220323
```

```bat
cmd
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
