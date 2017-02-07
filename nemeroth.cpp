#include <iostream>
#include "engine.hpp"

int main() {
    try {
        App app{};
        app.addClickListener([](Vec2f pos){
            std::cout << "Click: " << pos.x << ", " << pos.y << std::endl;
        });
        app.run();
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
