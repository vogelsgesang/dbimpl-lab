# Exercises for the [practical course on database implementation](http://www-db.in.tum.de/teaching/ws1516/imlab/index.shtml)

This repository contains the solutions for the tasks set during the [lab course on database implementation](http://www-db.in.tum.de/teaching/ws1516/imlab/index.shtml).

## Building

There a two types of builds: a debug build and a release build.
The release build is optimized for speed and does not contain debugging informations.
For the debug build all optimizations are turned off (`-O0`), debugging informations (`-g3`) are created and special debugging code is enabled (using `-DDEBUG`).

You can create a release build using `make`. For the debug build enter `make BUILD_TYPE=debug`.

The binaries will be created in the `bin/` folder.
Executables built with the debugging flags will have the `_debug` suffix.

## Testing

There are some rudimentary GTest testcases. In order to run them use `make test`.

## Schema Generation

`bin/generate_schema` reads an SQL schema file from stdin and writes corresponding C++ code to stdout. The schema generated for the TPC-C benchmark can be found in `schema/GeneratedSchema.hpp`.

## TPC-C benchmark

`bin/tpcc` contains a partial implementation of the TPC-C benchmark. In order to run it, additional files must be provided. The files are available as part of the [archive for task 1](www-db.in.tum.de/teaching/ws1516/imlab/task1.tgz) from the course webpage. The `.tbl` files from this archive should be put in the `data` folder.
