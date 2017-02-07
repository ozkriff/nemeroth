#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "vec2.hpp"

class Context {
    SDL_Renderer* renderer_;
    SDL_Window* window_;

public:
    const SDL_Renderer& renderer() const { return *renderer_; }
    SDL_Renderer& renderer() { return *renderer_; }

    const SDL_Window& window() const { return *window_; }
    SDL_Window& window() { return *window_; }

    Vec2i window_size();

    // пересортировать наверх
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
    void draw_at(Context& context, const Vec2f& pos) const;
    const Vec2i& size() const;
};

typedef std::function<void(Vec2f)> ClickCallback;

class App {
    Context context_;

    // TODO: вынести эти три
    Vec2f sprite_pos_;
    Vec2f mouse_pos_;
    Image image_;

    // TODO: и добавить сцену с геттерами

    bool is_running_;
    std::function<void(Vec2f)> click_callback_;

    void draw_();
    void process_input_();
    void update_sprite_pos_();
    void tick_();

#ifdef __EMSCRIPTEN__
    static void emscripten_tick_(void* arg);
#endif

public:
    App();

    void addClickListener(ClickCallback callback);
    void run();
};

#endif
