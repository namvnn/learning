.PHONY: fmt
fmt: fmt-c fmt-rs

.PHONY: fmt-c
fmt-c:
	@./scripts/fmt-c

.PHONY: fmt-rs
fmt-rs:
	@./scripts/fmt-rs
