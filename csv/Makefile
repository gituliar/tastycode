.PHONY: build check clean config


build:
	cmake --build --preset linux-release

config:
	cmake --preset linux-release

check: build
	cd test && ../.out/linux-release/main_test

clean:
	rm -rf .out
