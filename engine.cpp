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

void Image::draw_at(Context& context, const Vec2f& pos) const {
    const Vec2i pos_i = {
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
    };
    draw_at(context, pos_i);
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
  : context_{},
    sprite_pos_{200, 100},
    mouse_pos_{sprite_pos_},
    image_{context_, std::string{"assets/daemon.png"}}, // TODO: extract from engine
    is_running_{true}
{}

void App::tick() {
    process_input();
    update_sprite_pos_();
    draw();
}

void App::draw() {
    SDL_RenderClear(&context_.renderer());
    image_.draw_at(context_, sprite_pos_);
    SDL_RenderPresent(&context_.renderer());
}

void App::process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running_ = false;
                break;
            case SDL_MOUSEMOTION:
                printf("mouse moved\n");
                break;
            case SDL_MOUSEBUTTONUP:
                printf("mouse up\n");
                mouse_pos_ = {
                    static_cast<float>(event.motion.x),
                    static_cast<float>(event.motion.y),
                };
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("mouse down\n");
                break;
            case SDL_KEYDOWN:
                printf("key\n");
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        is_running_ = false;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void App::update_sprite_pos_() {
    const Vec2f diff = sprite_pos_ - mouse_pos_;
    const Vec2f sprite_velocity = -diff / 10.0;
    sprite_pos_ += sprite_velocity;
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
    while (is_running_) {
        tick();
        SDL_Delay(1000 / 60);
    }
}
#endif
