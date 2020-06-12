#include <set>
#include "main.h"

using namespace shipping;
int main(int argc, char **argv){
    int sum = 0;
    sum += test1();
    sum += test2();
    sum += test3();
    sum += test4();
    sum += test5();
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
        if(e.getMessage() != "No place to insert the container.") {
            std::cout << 2 << std::endl;
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
    ship->load(X{1}, Y{1}, "sprr");

    std::vector<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> v1 =
            {std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{0}, Y{1}, Height{0} ,"str"),
            std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{1}, Y{1}, Height{0} ,"sprr")};

    auto view_Hh = ship->getContainersViewByGroup("first_letter_toupper", "S");

    std::vector<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> v2;
    for(const auto& container_tuple : view_Hh) {
        v2.emplace_back(
                std::get<0>(container_tuple.first), std::get<1>(container_tuple.first), std::get<2>(container_tuple.first) ,container_tuple.second);
    }

    std::set<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> s1;
    s1.insert(v1.begin(), v1.end());

    std::set<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> s2;
    s2.insert(v2.begin(), v2.end());

    if(s1 == s2)
        return 1;

    return 0;
}

int test4() {
    Ship<int> ship{X{3}, Y{2}, Height{2}};

    ship.load(X{0}, Y{1}, 1);

    ship.load(X{0}, Y{1}, 3);
    ship.load(X{0}, Y{0}, 4);

    std::vector<int> v1 = {1, 3, 4};

    std::vector<int> v2;
    for (const auto& container : ship) {
        v2.push_back(container);
    }

    std::set<int> s1;
    s1.insert(v1.begin(), v1.end());

    std::set<int> s2;
    s2.insert(v2.begin(), v2.end());

    if(s1 == s2)
        return 1;

    return 0;
}

int test5() {
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X, Y, Height>(X{0}, Y{0}, Height{0}),
    };

    Ship<std::string> ship {X{3}, Y{2}, Height{2}, restrictions};

    ship.load(X{0}, Y{1}, "str");
    ship.load(X{0}, Y{1}, "sprr");
    ship.load(X{2}, Y{1}, "not_include");

    std::vector<std::string> v1 = {"sprr", "str"};

    auto view00 = ship.getContainersViewByPosition(X{0}, Y{1});

    std::vector<std::string> v2;
    for(const auto& container : view00) {
        v2.push_back(container);
    }

    if(v1 == v2)
        return 1;

    return 0;
}