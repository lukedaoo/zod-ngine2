NATIVE_FILE := native.ini
STATE_FILE := .config

BUILD_DIR = $(shell cat $(STATE_FILE) 2>/dev/null || echo build-debug)

.PHONY: debug release build run clean

debug:
	sed -i "s/^buildtype = .*/buildtype = 'debug'/" $(NATIVE_FILE)
	echo build-debug > $(STATE_FILE)
	meson setup build-debug --reconfigure --native-file $(NATIVE_FILE)

release:
	sed -i "s/^buildtype = .*/buildtype = 'release'/" $(NATIVE_FILE)
	echo build-release > $(STATE_FILE)
	meson setup build-release --reconfigure --native-file $(NATIVE_FILE)

build:
	ninja -C $(BUILD_DIR)

run: build
	$(BUILD_DIR)/zod-ngine2

test: build
	meson test -C $(BUILD_DIR)

clean:
	rm -rf build-debug build-release $(STATE_FILE)
