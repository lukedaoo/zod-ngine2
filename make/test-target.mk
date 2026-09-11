test: build
	meson test -C $(BUILD_DIR)

test-file: build
	meson test -C $(BUILD_DIR) $(FILE)

test-suite: build
	$(BUILD_DIR)/tests/$(FILE) $(SUITE)

test-tag: build
	meson test -C $(BUILD_DIR) --suite $(SUITE)

CXX := g++
CXX_STD := c++23
UNITTEST_VERSION := 2.0.0
UNITTEST_INC := subprojects/unittest-cpp-$(UNITTEST_VERSION)
UNITTEST_LIB := $(BUILD_DIR)/subprojects/unittest-cpp-$(UNITTEST_VERSION)/libunittest-cpp.a

test-cxx: build
	$(CXX) -std=$(CXX_STD) -Isrc -I$(UNITTEST_INC) \
		tests/test_main.cpp tests/$(FILE).cpp $(UNITTEST_LIB) \
		-o /tmp/$(FILE)
	/tmp/$(FILE) $(SUITE)

