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

compdb: build
	ninja -C $(BUILD_DIR) -t compdb > compile_commands.json

clean:
	rm -rf build-debug build-release $(STATE_FILE)
