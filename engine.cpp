// TODO: надо весь эмскриптен и сдл скрыть в "движке"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "engine.hpp"

Image::Image()
  : dest{},
    tex{nullptr}
{
    SDL_Surface* image = IMG_Load("assets/daemon.png"); // TODO: Обернуть в свой тип
    if (!image) {
         printf("IMG_Load: %s\n", IMG_GetError());
         // return 0;
         // TODO: throw ...
    }
    tex = SDL_CreateTextureFromSurface(renderer, image);
    dest.w = image->w;
    dest.h = image->h;
    SDL_FreeSurface(image);
}

Image::~Image() {
    // TODO: освободить текстурку
}

// TODO: исключения вмето инта
/// Loads the texture into the Context
int Context::get_texture() {
    SDL_Surface* image = IMG_Load("assets/daemon.png"); // TODO: Обернуть в свой тип
    if (!image) {
         printf("IMG_Load: %s\n", IMG_GetError());
         return 0;
    }
    tex = SDL_CreateTextureFromSurface(renderer, image);
    dest.w = image->w;
    dest.h = image->h;
    SDL_FreeSurface(image);
    return 1;
}

Context::Context()
  : renderer{nullptr},
    window{nullptr},
    dest{},
    tex{nullptr},
    active_state{NOTHING_PRESSED},
    vx{0},
    vy{0},
    is_running{true}
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(600, 400, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    get_texture();
    dest.x = 200;
    dest.y = 100;
}

void Context::tick() {
    process_input();
    dest.x += vx;
    dest.y += vy;
    draw();
}

void Context::draw() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    SDL_RenderPresent(renderer);
}

void Context::process_input() {
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
    vy = 0;
    vx = 0;
    if (active_state & UP_PRESSED) {
        vy = -5;
    }
    if (active_state & DOWN_PRESSED) {
        vy = 5;
    }
    if (active_state & LEFT_PRESSED) {
        vx = -5;
    }
    if (active_state & RIGHT_PRESSED) {
        vx = 5;
    }
    if (vx != 0 && vy != 0) {
        vx *= REC_SQRT2;
        vy *= REC_SQRT2;
    }
}

#ifdef __EMSCRIPTEN__
static void emscripten_tick(void* arg) {
    Context* context = static_cast<Context*>(arg);
    context->tick();
}

void Context::run() {
    emscripten_set_main_loop_arg(emscripten_tick, this, -1, 1);
}
#else
void Context::run() {
    while (is_running) {
        tick();
        SDL_Delay(1000 / 60);
    }
}
#endif
