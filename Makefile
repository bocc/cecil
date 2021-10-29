.PHONY: build
build: src/server.c
	@mkdir -p target
	@clang src/server.c -o target/server -Wall -pthread

.PHONY: serve
serve: build
	@target/server

.PHONY: test
test: build
	@target/server & echo $$! > server.PID
	@cargo run --manifest-path=test-client/Cargo.toml
	@pkill -F server.PID && rm server.PID
  
.PHONY: clean
clean:
	@rm -rf target/
