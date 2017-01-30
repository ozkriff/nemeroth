CXX_FLAGS=-O2 -std=c++14 -Wall -Wextra -pedantic
SOURCES=engine.cpp nemeroth.cpp
INCLUDES=engine.hpp
EMCC_LIBS=-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'

all: $(SOURCES) $(INCLUDES)
	clang++ $(CXX_FLAGS) $(SOURCES) -o nemeroth -lSDL2 -lSDL2_image

html: $(SOURCES) $(INCLUDES)
	em++ $(CXX_FLAGS) $(SOURCES) $(EMCC_LIBS) --preload-file assets -o nemeroth.html
