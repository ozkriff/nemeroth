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
    draw_at(context, to_int(pos));
}

const Vec2i& Image::size() const {
    return size_;
}

// TODO: SDL_WINDOW_RESIZABLE?
Context::Context()
  : renderer_{nullptr},
    window_{nullptr}
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1024, 768, 0, &window_, &renderer_);
    SDL_SetRenderDrawColor(renderer_, 0, 0, 150, 255);
}

Context::~Context() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

Vec2i Context::window_size() {
    int h, w;
    SDL_GetWindowSize(&window(), &w, &h);
    return Vec2i{w, h};
}

App::App()
  : context_{},
    sprite_pos_{200, 100},
    mouse_pos_{sprite_pos_},
    image_{context_, std::string{"assets/daemon.png"}}, // TODO: extract from engine
    is_running_{true},
    click_callback_{nullptr}
{}

void App::tick_() {
    process_input_();
    update_sprite_pos_();
    draw_();
}

void App::draw_() {
    SDL_RenderClear(&context_.renderer());
    image_.draw_at(context_, sprite_pos_);
    for (const auto& sprite : scene.sprites) {
        sprite->image->draw_at(context_, sprite->pos);
    }
    SDL_RenderPresent(&context_.renderer());
}

void App::process_input_() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running_ = false;
                break;
            case SDL_FINGERDOWN:
                break;
            case SDL_FINGERUP: {
                mouse_pos_ = Vec2f{event.tfinger.x, event.tfinger.y}
                    * to_float(context_.window_size());
                if (click_callback_) {
                    click_callback_(mouse_pos_);
                }
                break;
            }
            case SDL_FINGERMOTION:
                break;
            case SDL_MOUSEMOTION:
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.which == SDL_TOUCH_MOUSEID) {
                    continue;
                }
                mouse_pos_ = to_float(Vec2i{event.button.x, event.button.y});
                if (click_callback_) {
                    click_callback_(mouse_pos_);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_KEYDOWN:
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
    const Vec2f diff = mouse_pos_ - sprite_pos_ ;
    const Vec2f sprite_velocity = diff / 10.0;
    sprite_pos_ += sprite_velocity;
}

void App::addClickListener(ClickCallback callback) {
    click_callback_ = callback;
}

#ifdef __EMSCRIPTEN__
void App::emscripten_tick_(void* arg) {
    App* app = static_cast<App*>(arg);
    app->tick_();
}

void App::run() {
    bool simulate_infinite_loop = true;

    // Number of frames per second at which the JavaScript will call the
    // function. Setting int <=0 (recommended) uses the browser’s
    // requestAnimationFrame mechanism to call the function.
    int fps = 0;

    emscripten_set_main_loop_arg(
        emscripten_tick_, this, fps, simulate_infinite_loop);
}
#else
void App::run() {
    while (is_running_) {
        tick_(); // TODO: pass deltatime
        SDL_Delay(1000 / 60); // TODO: усыплять только на оставшееся время
    }
}
#endif
