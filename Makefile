CXX_FLAGS += -O2
CXX_FLAGS += -std=c++14
CXX_FLAGS += -Wall -Wextra -pedantic

EMCC_FLAGS += $(CXX_FLAGS)
EMCC_FLAGS += -s USE_SDL=2
EMCC_FLAGS += -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'
EMCC_FLAGS += -s DISABLE_EXCEPTION_CATCHING=0
EMCC_FLAGS += --preload-file assets

NATIVE_FLAGS += $(CXX_FLAGS)
NATIVE_FLAGS += -lSDL2 -lSDL2_image

SOURCES = engine.cpp nemeroth.cpp
HEADERS = engine.hpp

all: $(SOURCES) $(HEADERS)
	clang++ $(SOURCES) $(NATIVE_FLAGS) -o nemeroth

html: $(SOURCES) $(HEADERS)
	em++ $(SOURCES) $(EMCC_FLAGS) -o nemeroth.html
