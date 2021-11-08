.PHONY: build
build: src/server.c
	@mkdir -p target
	@clang src/server.c -o target/server -Wall -Werror -pthread

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
	@cargo clean --manifest-path=test-client/Cargo.toml
	@rm -rf target/

.PHONY: fmt
fmt:
	@clang-format -i src/server.c

.PHONY: leakcheck
leakcheck: build
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=target/valgrind_report.txt ./target/server