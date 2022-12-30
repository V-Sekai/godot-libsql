# godot-sqlite

[SQLite3](https://www.sqlite.org/) wrapper for Godot Engine

## Dependencies

* rust
* https://docs.godotengine.org/en/latest/development/compiling/index.html
* Foundatiodb https://github.com/apple/foundationdb/releases

## Git Installation

Download using git.

```
git clone https://github.com/V-Sekai/godot-mvsqlite mvsqlite
```

Move to the modules folder inside of the Godot Engine source.

```
mv mvsqlite godot/modules/
```

## Dependencies on Rocky Linux 8

```bash
sudo dnf config-manager --set-enabled powertools
sudo yum install mingw32-pkg-config mingw64-openssl glibc-static
```

## Example usage

- [SQL Queries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/sql_queries.gd)
- [Game Highscore](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/game_highscore.gd)
- [Item Libraries](https://github.com/godot-extended-libraries/godot-sqlite/blob/master/demo/SQLite/item_database.gd)

## Attribution

This engine module for Godot is based on gdsqlite-native by Khairul Hidayat in 2017.
