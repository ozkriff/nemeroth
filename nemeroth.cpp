#include <iostream>
#include "engine.hpp"

int main() {
    try {
        App app{};
        app.run();
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
