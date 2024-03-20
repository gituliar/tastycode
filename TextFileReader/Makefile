.PHONY: build check clean config profile


all: config build check

build:
	cmake --build --preset linux-release

config:
	cmake --preset linux-release

check:
	./.out/linux-release/TextFileReader_test
profile:
	./.out/linux-release/TextFileReader_profile profile/10_1000.txt

clean:
	rm -rf .out
