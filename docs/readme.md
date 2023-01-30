# Design docs

What is mvsqlite?

> mvSQLite is the open-source, SQLite-compatible distributed database built on FoundationDB.

### Keep status quo client deployment story:

1. Player story. The ability to have a single executable for playing V-Sekai.
1. Godot Engine has a mode called embedded mode and it merges the "pck" into the godot engine binary
1. This implies that gdextension shared libraries (.dll, .so) should not be in the base deployment because it is more than a single binary

### Easier server deployment:

1. Download a single zip-like archive
1. Unpack into components.
   1. the godot engine templates (Windows, Linux, macOS, and others)
   2. the V-Sekai Uro phoenix package
   3. the Foundationdb server
   4. the mvsqlite mvstore Foundationdb host
1. Docker is not required.

## Server to Client simulation

1. V-Sekai Godot Engine game servers have ~100 clients each.

### Avoiding hops in networking:

Transfer entities between Godot Engine servers.

Cold start Godot Engine clients.

Can store any Godot Engine variant.

1. Aim to have no double hops between the V-Sekai Uro backend, the godot engine and the godot game during simulation
1. V-Sekai Uro backend reads and writes to the mvstore HTTPS (TCP)
1. V-Sekai game servers can hibernate entities to the mvstore HTTPS (TCP)
1. V-Sekai game servers can fetch hibernated entities from the mvstore HTTPS (TCP)
1. V-Sekai game clients can fetch hibernated entities from the mvstore HTTPS (TCP)
1. V-Sekai game clients can instance entities from the mvstore HTTPS (TCP) on their first load

### Why mvsqlite and Foundationdb?:

1. Foundationdb is Apache licensed
1. Cockroachdb (our current storage component) is BSL and is not open-source.
1. mvsqlite provides a SQL layer to Foundationdb
1. mvsqlite supports 100 server processes with reasonable scaling
1. FoundationDB supports 500 server processes with reasonable scaling

### Explore using the datastore as the proxy between the game server and the game client

Some benchmarks mention 3 ms average with a max of 1.5 seconds.

### Explore using the datastore with UDP based HTTP protocols.

To avoid TCP packet blocking

### Use as data recovery

Sync every frame without acknledgement.

### Assistent entity transfer

Server assisted transfer of players.

I am on server 1 and I want to teleport to server 2.

### List of ideas

| Who     | Idea                                        | Evaluation                        |
| ------- | ------------------------------------------- | --------------------------------- |
| Fire    | World persistence                           | Definitely                        |
| Fire    | Frame proxying                              | Bad performance                   |
| Fire    | Inventory                                   | Can save any Variant or ByteArray |
| 74hc595 | Movie mode record and replay player actions | Movies! Yeah!                     |

### References

https://chrisseaton.com/army/ explained the difference between goals and tactics.

### Benchmarking

https://stroppy.io/reports/fdb-report.html

https://github.com/losfair/mvsqlite/wiki/YCSB-numbers

https://univalence.me/posts/mvsqlite-2
