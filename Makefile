CXX_FLAGS += -O2
CXX_FLAGS += -std=c++14
CXX_FLAGS += -Wall -Wextra -Wconversion -pedantic

EMCC_FLAGS += -s USE_SDL=2
EMCC_FLAGS += -s USE_SDL_IMAGE=2
EMCC_FLAGS += -s SDL2_IMAGE_FORMATS='["png"]'
EMCC_FLAGS += -s DISABLE_EXCEPTION_CATCHING=0
EMCC_FLAGS += --preload-file assets

NATIVE_FLAGS += -lSDL2_image
NATIVE_FLAGS += -stdlib=libc++
NATIVE_FLAGS += `pkg-config --cflags --libs sdl2 SDL2_image`

SOURCES = engine.cpp nemeroth.cpp
HEADERS = engine.hpp

all: $(SOURCES) $(HEADERS)
	clang++ -o nemeroth $(NATIVE_FLAGS) $(SOURCES) $(CXX_FLAGS)

html: $(SOURCES) $(HEADERS)
	em++ -o index.html $(EMCC_FLAGS) $(SOURCES) $(CXX_FLAGS)
