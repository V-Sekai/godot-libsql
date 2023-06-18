# godot-libsql

A custom Godot Engine module to use the SQLite relational database.

## Table of Contents

- [How to Compile](#how-to-compile)
- [Example Usage](#example-usage)
- [Creating WASM Programs](#creating-wasm-programs)
- [Installation for Windows](#installation-for-windows)
- [Refreshing sqlite3.c](#refreshing-sqlite3c)
- [Attribution](#attribution)

## How to Compile

- [Godot Engine Compiling Documentation](https://docs.godotengine.org/en/latest/development/compiling/index.html)
- OPTIONAL: Rust for cluster database support

## Example Usage

- [SQL Queries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/sql_queries.gd)
- [Game Highscore](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/game_highscore.gd)
- [Item Libraries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/item_database.gd)

## Creating WASM Programs

[Bindgen LibSQL](https://bindgen.libsql.org/)

## Installation for Windows

```bat
cmd
scoop install llvm-mingw@20220906
scoop install rustup
rustup update
rustup target remove x86_64-pc-windows-msvc
rustup target add x86_64-pc-windows-gnu
rustup default stable-x86_64-pc-windows-gnu
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

## Attribution

This engine module for Godot is based on gdsqlite-native by Khairul Hidayat in 2017.

Thanks to the libsql team for their work in 2023.