#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "vec2.hpp"

// TODO: add namespace

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

struct Sprite {
    std::shared_ptr<Image> image;
    Vec2f pos;

    Sprite(std::shared_ptr<Image> image, Vec2f pos):
        image{image},
        pos{pos}
    {}
};

// TODO: http://cocos2d-x.org/docs/programmers-guide/actions и т.п.
class Action {
public:
    virtual ~Action() {}

    // надо мне вообще спрайт сюда передавать?
    virtual void act(Sprite& sprite) = 0;
    virtual bool is_finished() = 0;
};

class ActionMove: Action {
public:
    ActionMove() {}

    virtual void act(Sprite& sprite) override {
        // TODO: пихать в реально нужном направлении
        sprite.pos.x += 1;
    }

    virtual bool is_finished() override {
        return false; // TODO: время-длину там проверять
        // может есть смысл изначально время считать, что бы
        // только одна виртуальная функция была
    }
};

struct Scene {
    std::vector<std::shared_ptr<Sprite>> sprites;
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
    ClickCallback click_callback_;

    void draw_();
    void process_input_();
    void update_sprite_pos_(); // TODO удалить
    void tick_();

#ifdef __EMSCRIPTEN__
    static void emscripten_tick_(void* arg);
#endif

public:
    App();

     // TODO скрыть
    Scene scene;
    std::vector<std::unique_ptr<Action>> actions;

    void addClickListener(ClickCallback callback);
    void run();

    // Нужно ли это выставлять напоказ?
    // Скорее нужен какой-то менеджер ресурсов
    Context& context() { return context_; }
    const Context& context() const { return context_; }
};

#endif
