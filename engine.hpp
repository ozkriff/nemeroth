#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/// Inverse square root of two, for normalising velocity
constexpr float REC_SQRT2 = 0.7071067811865475;

/// Set of input states
enum InputState {
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

class Image {
    SDL_Rect dest;
    SDL_Texture* tex;
public:
    Image();
    ~Image();

    void draw();
};

/// Context structure that will be passed to the loop handler
class Context {
    SDL_Renderer* renderer;
    SDL_Window* window;

    /// Rectangle that the owl texture will be rendered into
    SDL_Rect dest;
    SDL_Texture* tex;

    int active_state; // TODO: InputState active_state

    /// x and y components of owl's velocity
    int vx;
    int vy;

    bool is_running;

    void draw();
    void process_input();
    int get_texture();

public:
    Context();
    void tick();
    void run();
};


