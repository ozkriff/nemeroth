CXX_FLAGS += -O2
CXX_FLAGS += -std=c++1y
CXX_FLAGS += -Wall -Wextra -Wconversion -pedantic

EMCC_FLAGS += -s USE_SDL=2
EMCC_FLAGS += -s USE_SDL_IMAGE=2
EMCC_FLAGS += -s SDL2_IMAGE_FORMATS='["png"]'
EMCC_FLAGS += -s DISABLE_EXCEPTION_CATCHING=0
EMCC_FLAGS += --preload-file assets

NATIVE_FLAGS += `pkg-config --cflags --libs sdl2 SDL2_image`

SOURCES = engine.cpp nemeroth.cpp
HEADERS = engine.hpp

all: $(SOURCES) $(HEADERS)
	$(CXX) -o nemeroth $(CXX_FLAGS) $(NATIVE_FLAGS) $(SOURCES)

html: $(SOURCES) $(HEADERS)
	em++ -o index.html $(CXX_FLAGS) $(EMCC_FLAGS) $(SOURCES)
