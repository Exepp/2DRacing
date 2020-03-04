#include <GGEngine/Game.h>
#include <iostream>


int main()
try {
    Game::Run();
    return 0;
}
catch (const std::exception& exception) {
    std::cout << "Catched exception: " << exception.what() << std::endl;
    return 1;
}