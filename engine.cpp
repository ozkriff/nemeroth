// TODO: надо весь эмскриптен и сдл скрыть в "движке"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdexcept>
#include "engine.hpp"

Image::Image(Context& context, const std::string& name)
  : size_{},
    tex_{nullptr}
{
    SDL_Surface* image = IMG_Load(name.c_str());
    if (!image) {
         throw std::runtime_error(IMG_GetError());
    }
    tex_ = SDL_CreateTextureFromSurface(&context.renderer(), image);
    size_.x = image->w;
    size_.y = image->h;
    SDL_FreeSurface(image);
}

Image::~Image() {
    SDL_DestroyTexture(tex_);
}

void Image::draw_at(Context& context, const Vec2i& pos) const {
    const SDL_Rect dest{pos.x, pos.y, size().x, size().y};
    SDL_RenderCopy(&context.renderer(), tex_, nullptr, &dest);
}

const Vec2i& Image::size() const {
    return size_;
}

Context::Context()
  : renderer_{nullptr},
    window_{nullptr}
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(600, 400, 0, &window_, &renderer_);
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
}

Context::~Context() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

App::App()
  : context{},
    sprite_pos{200, 100},
    sprite_velocity{0, 0},
    image{context, std::string{"assets/daemon.png"}}, // TODO: extract from engine
    active_state{NOTHING_PRESSED},
    is_running{true}
{}

void App::tick() {
    process_input();
    sprite_pos += sprite_velocity;
    draw();
}

void App::draw() {
    SDL_RenderClear(&context.renderer());
    image.draw_at(context, sprite_pos);
    SDL_RenderPresent(&context.renderer());
}

void App::process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            is_running = false;
        }
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN) {
                    active_state |= UP_PRESSED;
                } else if (event.key.type == SDL_KEYUP) {
                    active_state ^= UP_PRESSED;
                }
                break;
            case SDLK_DOWN:
                if (event.key.type == SDL_KEYDOWN) {
                    active_state |= DOWN_PRESSED;
                } else if (event.key.type == SDL_KEYUP) {
                    active_state ^= DOWN_PRESSED;
                }
                break;
            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN)
                    active_state |= LEFT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    active_state ^= LEFT_PRESSED;
                break;
            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN)
                    active_state |= RIGHT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    active_state ^= RIGHT_PRESSED;
                break;
            case SDLK_q:
            case SDLK_ESCAPE:
                is_running = false;
            default:
                break;
        }
    }
    sprite_velocity = Vec2i{};
    if (active_state & UP_PRESSED) {
        sprite_velocity.y = -5;
    }
    if (active_state & DOWN_PRESSED) {
        sprite_velocity.y = 5;
    }
    if (active_state & LEFT_PRESSED) {
        sprite_velocity.x = -5;
    }
    if (active_state & RIGHT_PRESSED) {
        sprite_velocity.x = 5;
    }
}

#ifdef __EMSCRIPTEN__
static void emscripten_tick(void* arg) {
    App* app = static_cast<App*>(arg);
    app->tick();
}

void App::run() {
    bool simulate_infinite_loop = true;

    // Number of frames per second at which the JavaScript will call the
    // function. Setting int <=0 (recommended) uses the browser’s
    // requestAnimationFrame mechanism to call the function.
    int fps = 0;

    emscripten_set_main_loop_arg(
        emscripten_tick, this, fps, simulate_infinite_loop);
}
#else
void App::run() {
    while (is_running) {
        tick();
        SDL_Delay(1000 / 60);
    }
}
#endif
