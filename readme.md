# cecil - a TCP server for echoing reversed lines

## Usage
Connect to it via TCP, send it \n delimited bytes, and you'll receive them reversed.
When you're done, close the connection.

## Make Targets
- build (requires `clang`)
- serve (for requirements, see `build`)
- test (requires a `Rust` toolchain)
- clean
- fmt (requires `clang-format`)
- leakcheck (requires `valgrind`)

## License
Just make sure you don't sue me.