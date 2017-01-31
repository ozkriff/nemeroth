#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "vec2.hpp"

enum InputState {
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

class Context {
    SDL_Renderer* renderer_;
    SDL_Window* window_;

public:
    const SDL_Renderer& renderer() const { return *renderer_; }
    SDL_Renderer& renderer() { return *renderer_; }

    Context();
    ~Context();
};

class Image {
    Vec2i size_;
    SDL_Texture* tex_;

public:
    Image(Context& context, const std::string& name);
    ~Image();

    void draw_at(Context& context, const Vec2i& pos) const;
    const Vec2i& size() const;
};

class App {
    Context context;

    Vec2i sprite_pos;
    Vec2i sprite_velocity;
    Image image;

    int active_state; // TODO: InputState active_state


    bool is_running;

    void draw();
    void process_input();

public:
    App();
    void tick();
    void run();
};

#endif
