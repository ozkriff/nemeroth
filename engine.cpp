// TODO: надо весь эмскриптен и сдл скрыть в "движке"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdexcept>
#include "engine.hpp"

Image::Image(const Context& context, const std::string& name)
  : dest{},
    tex{nullptr}
{
    SDL_Surface* image = IMG_Load(name.c_str());
    if (!image) {
         throw std::runtime_error(IMG_GetError());
    }
    tex = SDL_CreateTextureFromSurface(context.renderer, image);
    dest.w = image->w;
    dest.h = image->h;
    SDL_FreeSurface(image);
}

Image::~Image() {
    SDL_DestroyTexture(tex);
}

void Image::draw(const Context& context) {
    SDL_RenderCopy(context.renderer, tex, nullptr, &dest);
}

Context::Context()
  : renderer{nullptr},
    window{nullptr}
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(600, 400, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

Context::~Context() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

App::App()
  : context{},
    image{context, std::string{"assets/daemon.png"}},
    active_state{NOTHING_PRESSED},
    sprite_pos{0, 0},
    is_running{true}
{
    image.dest.x = 200;
    image.dest.y = 100;
}

void App::tick() {
    process_input();
    image.dest.x += sprite_pos.x;
    image.dest.y += sprite_pos.y;
    draw();
}

void App::draw() {
    SDL_RenderClear(context.renderer);
    image.draw(context);
    SDL_RenderPresent(context.renderer);
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
    sprite_pos = Vec2<int>{};
    if (active_state & UP_PRESSED) {
        sprite_pos.y = -5;
    }
    if (active_state & DOWN_PRESSED) {
        sprite_pos.y = 5;
    }
    if (active_state & LEFT_PRESSED) {
        sprite_pos.x = -5;
    }
    if (active_state & RIGHT_PRESSED) {
        sprite_pos.x = 5;
    }
    if (sprite_pos.x != 0 && sprite_pos.y != 0) {
        // Inverse square root of two, for normalising velocity
        constexpr float inv_sqrt_2 = 0.7071067811865475;

        sprite_pos *= inv_sqrt_2;
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
