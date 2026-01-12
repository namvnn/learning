.PHONY: fmt
fmt: fmt-c fmt-rust

.PHONY: fmt-c
fmt-c:
	@echo "Formatting C projects..."
	@find . -type f \
		\( -name '*.c' -o -name '*.h' \) \
		-not -path '*lib*' \
		-not -path '*.git*' \
		-exec clang-format -i {} '+'

.PHONY: fmt-rust
fmt-rust:
	@echo "Formatting Rust projects..."
	@for project in rustlings ; do \
		cd "$${project}" && cargo fmt --all -- --config-path ../rustfmt.toml ; \
    done
