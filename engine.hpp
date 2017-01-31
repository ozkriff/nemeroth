#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/// Inverse square root of two, for normalising velocity
constexpr float REC_SQRT2 = 0.7071067811865475;

enum InputState {
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

struct Context {
    SDL_Renderer* renderer;
    SDL_Window* window;

    // TODO: const SDL_Renderer& renderer() const;

    Context();
    ~Context();
};

class Image {
public: // TODO
    SDL_Rect dest; // TODO убрать
    SDL_Texture* tex;

public:
    Image(const Context& context, const std::string& name);
    ~Image();

    void draw(const Context& context);
};

class App {
    Context context;

    // SDL_Rect dest;
    Image image;

    int active_state; // TODO: InputState active_state

    int vx;
    int vy;

    bool is_running;

    void draw();
    void process_input();

public:
    App();
    void tick();
    void run();
};


