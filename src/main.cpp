#include "main.h"

using namespace shipping;
int main(int argc, char **argv){
    int sum = 0;
    sum += test1();
    sum += test2();
    std::cout << sum << std::endl;
}

int test1(){
    // simple load, move, unload with restrictions.
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X, Y, Height>(X{0}, Y{0}, Height{0}),
    };

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2}, restrictions);
    try {
        ship->load(X{0}, Y{1}, "str");
    } catch (BadShipOperationException& e) {
        std::cout << 1 << std::endl;
        return 0;
    }

    try {
        ship->move(X{0}, Y{1}, X{0}, Y{0});
    } catch (BadShipOperationException& e) {
        if(e.getMessage() != "No place to insert the container.") {
            std::cout << 2.3 << std::endl;
            return 0;
        }
    }

    try {
        auto output = ship->unload(X{0}, Y{0});
    } catch (BadShipOperationException& e) {
        if(e.getMessage() != "No container to unload from this position.") {
            std::cout << 3 << std::endl;
            return 0;
        }
    }

    try {
        ship->move(X{0}, Y{1}, X{1}, Y{1});
    } catch (BadShipOperationException& e) {
        std::cout << 4 << std::endl;
        return 0;
    }

    try {
        auto output = ship->unload(X{1}, Y{1});
        if(output != "str"){
            std::cout << 5 << std::endl;
            return 0;
        }
    } catch (BadShipOperationException& e) {
        std::cout << 6 << std::endl;
        return 0;
    }
    return 1;
}

int test2(){
    // simple load, move, unload with restrictions and grouping.
    Grouping<std::string> groupingFunctions = {
            { "first_letter",
                    [](const std::string& s){ return std::string(1, s[0]); }
            },
            { "first_letter_toupper",
                    [](const std::string& s){ return std::string(1, char(std::toupper(s[0]))); }
            }
    };

    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X, Y, Height>(X{0}, Y{0}, Height{0}),
    };

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2}, restrictions, groupingFunctions);
    try {
        ship->load(X{0}, Y{1}, "str");
    } catch (BadShipOperationException& e) {
        std::cout << 1 << std::endl;
        return 0;
    }

    try {
        ship->move(X{0}, Y{1}, X{0}, Y{0});
    } catch (BadShipOperationException& e) {
        if(e.getMessage() != "No place to insert the container.") {
            std::cout << 2.3 << std::endl;
            return 0;
        }
    }

    try {
        auto output = ship->unload(X{0}, Y{0});
    } catch (BadShipOperationException& e) {
        if(e.getMessage() != "No container to unload from this position.") {
            std::cout << 3 << std::endl;
            return 0;
        }
    }

    try {
        ship->move(X{0}, Y{1}, X{1}, Y{1});
    } catch (BadShipOperationException& e) {
        std::cout << 4 << std::endl;
        return 0;
    }

    try {
        auto output = ship->unload(X{1}, Y{1});
        if(output != "str"){
            std::cout << 5 << std::endl;
            return 0;
        }
    } catch (BadShipOperationException& e) {
        std::cout << 6 << std::endl;
        return 0;
    }
    return 1;
}