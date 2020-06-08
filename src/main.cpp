#include "main.h"

using namespace shipping;
int main(int argc, char **argv){
    // create restrictions for specific locations on the ship
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X, Y, Height>(X{0}, Y{0}, Height{0}),
//            std::tuple<X, Y, Height>(X{2}, Y{7}, Height{1}),
//            std::tuple<X, Y, Height>(X{2}, Y{5}, Height{6}),
    };

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2},restrictions);
//    std::cout << ship->x << ", " << ship->y << ", " << ship->height << std::endl;
    ship->load(X{0}, Y{1}, "str");
    ship->move(X{0}, Y{1}, X{0}, Y{0});
////    ship->load(X{0}, Y{1}, "str");
    std::cout << ship->unload(X{0}, Y{0}) << std::endl;
//    std::cout << ship->unload(X{0}, Y{1}) << std::endl;
//    ship->load(X{0}, Y{1}, "str");



//    std::cout << ship->findHeightIndex(X{0}, Y{1}) << std::endl;
//    std::cout << ship->x << ", " << ship->y << ", " << ship->height << std::endl;
}
