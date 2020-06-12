#include "main.h"

using namespace shipping;
int main(int argc, char **argv){
    int sum = 0;
//    sum += test1();
//    sum += test2();
//    sum += test3();
    sum += test4();
    std::cout << sum << std::endl;
}

int test1() {
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

    }

    try {
        auto output = ship->unload(X{0}, Y{0});
    } catch (BadShipOperationException& e) {

    }

    try {
        ship->move(X{0}, Y{1}, X{1}, Y{1});
    } catch (BadShipOperationException& e) {
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

int test2() {
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

    }

    try {
        auto output = ship->unload(X{0}, Y{0});
    } catch (BadShipOperationException& e) {

    }

    try {
        ship->move(X{0}, Y{1}, X{1}, Y{1});
    } catch (BadShipOperationException& e) {
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

int test3() {
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

    ship->load(X{0}, Y{1}, "str");
    ship->load(X{0}, Y{1}, "sprr");

    auto view_Hh = ship->getContainersViewByGroup("first_letter_toupper", "S");
    std::cout << "start" << std::endl;
    for(const auto& container_tuple : view_Hh) {
        std::cout << "X: " << std::get<0>(container_tuple.first) << ", Y: " << std::get<1>(container_tuple.first) << ", Height: " << std::get<2>(container_tuple.first) << std::endl;
    }
    std::cout << "finish" << std::endl;
    return 1;
}

int test4() {
    Ship<int> ship{X{3}, Y{2}, Height{2}};

    ship.load(X{0}, Y{1}, 1);
//    std::cout <<  ship->unload(X{0}, Y{1}) << std::endl;
//    std::cout <<  1231234 << std::endl;
//    ship->load(X{0}, Y{1}, 3);
//    ship->load(X{0}, Y{0}, 4);
//    ship->load(X{0}, Y{1}, "str1");
    const auto& iter = ship.begin();
    std::cout <<  "iter value: " << *iter << std::endl;

    for (const auto& container : ship) {
//        printf("Address of x is %p\n", (void *)(&container));
        std::cout <<  "iter value: " << container << std::endl;
    }

    return 1;
}
//
//int test4() {
//    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2});
//
//    std::string st = "str";
//    ship->load(X{0}, Y{1}, st);
////    ship->load(X{0}, Y{1}, "str1");
//
//    auto view00 = ship->getContainersViewByPosition(X{0}, Y{1});
//
//    for(const auto& container : view00) {
//        std::cout << container << std::endl;
//    }
//
////    for (const auto & iter : *ship) {
////        std::string curString = iter;
////        printf("Address of x is %p\n", (void *)(&iter));
////        std::cout << iter << std::endl;
////    }
//
//    return 1;
//}