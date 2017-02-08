#include <iostream>
#include <memory>
#include "engine.hpp"

int main() {
    try {
        App app{};
        app.addClickListener([](Vec2f pos){
            std::cout << "Click: " << pos.x << ", " << pos.y << std::endl;
        });
        // auto img = std::make_shared(new Image{app.context(), "assets/daemon.png"});

        // надо бы загружать изображения каким-то менеджером,
        // что бы они там кешировались
        auto img = std::make_shared<Image>(app.context(), "assets/daemon.png");

        auto sprite = std::make_shared<Sprite>(img, Vec2f{50, 50});

        // тут нужен какой-то метод в духе `app.scene.attach(sprite);`
        app.scene.sprites.push_back(sprite);

        app.run();
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
