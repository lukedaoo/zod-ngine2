NATIVE_FILE := native.ini
STATE_FILE := .config

BUILD_DIR = $(shell cat $(STATE_FILE) 2>/dev/null || echo build-debug)

.PHONY: help debug release build run compdb test test-file test-suite test-tag test-no-build test-asan test-cxx demo clean

include make/build-target.mk
include make/test-target.mk
include make/demo-target.mk
include make/help.mk
