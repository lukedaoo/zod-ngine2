ifeq ($(OS),Windows_NT)
    EXE_EXT := .exe
else
    EXE_EXT :=
endif

DEMO_BIN := demo/$(FILE)$(EXE_EXT)
DEMO_SRC := $(wildcard src/zodlib/sys/mem/*.cpp) $(wildcard src/zodlib/container/*.cpp)

demo:
	$(CXX) -std=$(CXX_STD) -D_DEBUG -Isrc demo/$(FILE).cpp $(DEMO_SRC) -o $(DEMO_BIN)
	$(DEMO_BIN)
