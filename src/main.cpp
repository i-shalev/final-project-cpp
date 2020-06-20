#include <iostream>
#include "Ship.h"
using namespace shipping;
int test1(){
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X,Y,Height>(X{2}, Y{6}, Height{0}),
            std::tuple<X,Y,Height>(X{2}, Y{7}, Height{1}),
            std::tuple<X,Y,Height>(X{2}, Y{5}, Height{6}),
    };
    try {
        restrictions.push_back( std::tuple<X,Y,Height>(X{2}, Y{5}, Height{6}) );
        Ship<std::string> myShip{ X{4}, Y{12}, Height{16}, restrictions };
    } catch(BadShipOperationException& e) {
        if(e.getMessage().compare("Duplicate restrictions (whether or not it has same limit).")!=0)
            return 1;
        restrictions.pop_back(); // remove the duplicate restriction
    }
    try {
        Ship<std::string> myShip{ X{4}, Y{7}, Height{8}, restrictions };
    } catch(BadShipOperationException& e) {
        if(e.getMessage().compare("Y index out of range.")!=0)
            return 1;
    }
    try {
        Ship<std::string> myShip{ X{4}, Y{12}, Height{6}, restrictions };
    } catch(BadShipOperationException& e) {
        if(e.getMessage().compare("Restriction with height equal or greater than the original height.")!=0)
            return 1;
    }
    Ship<std::string> myShip{ X{4}, Y{8}, Height{8}, restrictions };
    try {
        myShip.load(X{2}, Y{6}, "Hello");
    } catch(BadShipOperationException& e) {
        if(e.getMessage().compare("No place to insert the container.")!=0)
            return 1;
    }
    myShip.load(X{2}, Y{7}, "Hello");
    try {
        myShip.load(X{2}, Y{7}, "Hello");
    } catch(BadShipOperationException& e) {
        if(e.getMessage().compare("No place to insert the container.")!=0)
            return 1;
    }
    try {
        std::string container = myShip.unload(X{1}, Y{1});
    } catch(BadShipOperationException& e) {
        if(e.getMessage().compare("No container to unload from this position.")!=0)
            return 1;
    }

    try {
        myShip.load(X{1}, Y{8}, "Hi");
    } catch(BadShipOperationException& e) {
        if(e.getMessage().compare("Y index out of range.")!=0)
            return 1;
    }
    std::string container = myShip.unload(X{2} ,Y{7});
    if(container.compare("Hello")!=0)
        return 1;
    return 0;
}

int test2(){
    Ship<int> myShip{ X{2}, Y{2}, Height{2}};
    try {
        myShip.move(X{0}, Y{0}, X{0}, Y{0});
    }catch (BadShipOperationException& e){
        if(e.getMessage().compare("No container to unload from this position.")!=0)
            return 1;
    }
    myShip.load(X{0}, Y{0} , 5);
    myShip.move(X{0},Y{0}, X{1}, Y{1});
    try {
        myShip.unload(X{0}, Y{0});
    }catch (BadShipOperationException& e){
        if(e.getMessage().compare("No container to unload from this position.")!=0)
            return 1;
    }
    myShip.load(X{0},Y{0},6);
    myShip.load(X{0},Y{0},7);
    try {
        myShip.move(X{1}, Y{1}, X{0}, Y{0});
    }catch (BadShipOperationException& e){
        if(e.getMessage().compare("No place to insert the container.")!=0)
            return 1;
    }
    if(myShip.unload(X{1}, Y{1}) != 5)
        return 1;
    myShip.move(X{0}, Y{0}, X{0}, Y{0});
    if(myShip.unload(X{0}, Y{0}) != 7)
        return 1;
    if(myShip.unload(X{0}, Y{0}) != 6)
        return 1;
    return 0;

}

int test3(){
    Ship<int> myShip{ X{2}, Y{2}, Height{3}};
    myShip.load(X{0}, Y{0}, 1);
    myShip.load(X{0}, Y{0}, 2);
    myShip.load(X{0}, Y{0}, 3);
    myShip.load(X{1}, Y{0}, 2);
    myShip.load(X{1}, Y{0}, 3);
    myShip.load(X{0}, Y{1}, 2);
    int i=3;
    auto itr = myShip.getContainersViewByPosition(X{0}, Y{0});
    for(auto& container:itr){
        if(container != i)
            return 1;
        i--;
    }
    i=3;
    auto itr2 = myShip.getContainersViewByPosition(X{1}, Y{0});
    for(auto& container:itr2){
        if(container != i)
            return 1;
        i--;
    }
    auto itr3 = myShip.getContainersViewByPosition(X{0}, Y{1});
    for(auto& container:itr3){
        if(container != 2)
            return 1;
    }
    auto itr4 = myShip.getContainersViewByPosition(X{1}, Y{1});
    for(auto& container:itr4){
        (void)container;
        //empty view
        return 1;
    }
    auto itr5 = myShip.getContainersViewByPosition(X{0}, Y{0}).begin();
    ++itr5;
    if(*(itr5) != 2)
        return 1;
    myShip.unload(X{0}, Y{0});
    myShip.unload(X{0},Y{0});
    myShip.load(X{0},Y{0}, 17);
    if(*(itr5) != 17)
        return 1;
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X,Y,Height>(X{0}, Y{0}, Height{3}),
            std::tuple<X,Y,Height>(X{0}, Y{1}, Height{3})
    };
    Ship<int> myShip2{ X{2}, Y{2}, Height{5}, restrictions};
    myShip2.load(X{0}, Y{0}, 1);
    myShip2.load(X{0}, Y{0}, 2);
    i=2;
    auto itr6 = myShip2.getContainersViewByPosition(X{0}, Y{0});
    for(auto& container:itr6){
        if(container != i)
            return 1;
        i--;
    }
    auto itr7 = myShip2.getContainersViewByPosition(X{0}, Y{1});
    for(auto& container:itr7){
        (void)container;
        return 1;
    }
    try{
        auto itr7 = myShip2.getContainersViewByPosition(X{80}, Y{1});
        (void)itr7;
    }
    catch (BadShipOperationException& e){
        if(e.getMessage().compare("X index out of range.")!=0)
            return 1;
    }
    return 0;
}

int test4(){
    Ship<int> myShip{ X{2}, Y{2}, Height{3}};
    myShip.load(X{0}, Y{0}, 1);
    myShip.load(X{0}, Y{0}, 2);
    myShip.load(X{0}, Y{0}, 3);
    myShip.load(X{1}, Y{0}, 4);
    myShip.load(X{1}, Y{0}, 5);
    myShip.load(X{0}, Y{1}, 6);
    myShip.load(X{0}, Y{1}, 7);
    myShip.load(X{1}, Y{1}, 8);
    int i=0;
    for(auto& elem:myShip){
        (void)elem;
        i++;
    }
    if(i!=8)
        return 1;
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X,Y,Height>(X{0}, Y{0}, Height{3}),
            std::tuple<X,Y,Height>(X{0}, Y{1}, Height{3})
    };
    Ship<int> myShip2{ X{2}, Y{2}, Height{5},restrictions};
    myShip2.load(X{0}, Y{0}, 6);
    myShip2.load(X{0}, Y{0}, 7);
    myShip2.load(X{1}, Y{1}, 8);
    myShip2.load(X{0}, Y{1}, 9);
    i=0;
    for(auto& elem:myShip2){
        (void)elem;
        i++;
    }
    if(i!=4)
        return 1;
    return 0;
}

int test5(){
    Ship<int> myShip{ X{2}, Y{2}, Height{3}};
    myShip.load(X{0}, Y{0}, 1);
    myShip.load(X{0}, Y{0}, 2);
    myShip.load(X{0}, Y{0}, 3);
    myShip.load(X{1}, Y{0}, 4);
    myShip.load(X{1}, Y{0}, 5);
    myShip.load(X{0}, Y{1}, 6);
    myShip.load(X{0}, Y{1}, 7);
    myShip.load(X{1}, Y{1}, 8);
    auto itr = myShip.begin();
    myShip.load(X{1}, Y{1}, 15);
    for(; itr!=myShip.end(); ++itr){
        if(*(itr) == 15) {
            return 0;
        }
    }
    return 1;
}

int test6(){
    Grouping<std::string> groupingFunctions = {
            { "first_letter",
                    [](const std::string& s){ return std::string(1, s[0]); }
            },
            { "first_letter_toupper",
                    [](const std::string& s){ return std::string(1, char(std::toupper(s[0]))); }
            }
    };
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X,Y,Height>(X{2}, Y{6}, Height{4}),
            std::tuple<X,Y,Height>(X{2}, Y{7}, Height{6}),
    };
    Ship<std::string> myShip{ X{5}, Y{12}, Height{8},
                              restrictions,
                              groupingFunctions };

    myShip.load(X{0}, Y{0}, "Hello");
    myShip.load(X{1}, Y{1}, "hey");
    myShip.load(X{1}, Y{1}, "bye");
    auto view00 = myShip.getContainersViewByPosition(X{0}, Y{0});
    auto view_h = myShip.getContainersViewByGroup("first_letter", "h");
    auto view_Hh = myShip.getContainersViewByGroup("first_letter_toupper", "H");
    myShip.load(X{0}, Y{0}, "hi");
    int i=0;
    for(const auto& container : myShip) {
        i++;
        (void)container;
    }
    if(i!=4)
        return 1;
    i=1;
    for(const auto& container : view00) {
        if(i==1){
            if(container!="hi")
                return 1;
        }
        else{
            if(container!="Hello")
                return 1;
        }
        i++;
    }

    // loop on view_h:	pair { tuple{X{0}, Y{0}, Height{1}}, hi },
    // 						pair { tuple{X{1}, Y{1}, Height{0}}, hey }
    // - in some undefined order
    i=0;
    for(const auto& container_tuple : view_h) {
        i++;
        (void)container_tuple;
    }
    if(i!=2)
        return 1;

    i=0;
    for(const auto& container_tuple : view_Hh) {
        i++;
        (void)container_tuple;
    }
    if(i!=3)
        return 1;
    return 0;
}

int test7(){
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X,Y,Height>(X{2}, Y{6}, Height{4}),
            std::tuple<X,Y,Height>(X{2}, Y{7}, Height{6}),
    };
    Ship<int> myShip{ X{2}, Y{2}, Height{3}};
    myShip.load(X{0}, Y{0}, 1);
    myShip.load(X{1}, Y{1}, 2);
    myShip.load(X{1}, Y{1}, 3);
    auto view00 = myShip.getContainersViewByPosition(X{0}, Y{0});
    myShip.load(X{0}, Y{0}, 4);
    int i=1;
    for(const auto& container : view00) {
        if(i==1){
            if(container!=4)
                return 1;
        }
        else{
            if(container!=1)
                return 1;
        }
        i++;
    }
    return 0;

}

int test8(){
    // simple load, move, unload with restrictions.
    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X, Y, Height>(X{0}, Y{0}, Height{0}),
    };

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2}, restrictions);
    try {
        ship->load(X{0}, Y{1}, "str");
    } catch (BadShipOperationException& e) {
        std::cout << 1 << std::endl;
        return 1;
    }

    try {
        ship->move(X{0}, Y{1}, X{0}, Y{0});
    } catch (BadShipOperationException& e) {
        if(e.getMessage() != "No place to insert the container.") {
            std::cout << 2 << std::endl;
            return 1;
        }
    }

    try {
        auto output = ship->unload(X{0}, Y{0});
    } catch (BadShipOperationException& e) {
        if(e.getMessage() != "No container to unload from this position.") {
            std::cout << 3 << std::endl;
            return 1;
        }
    }

    try {
        ship->move(X{0}, Y{1}, X{1}, Y{1});
    } catch (BadShipOperationException& e) {
        return 1;
    }

    try {
        auto output = ship->unload(X{1}, Y{1});
        if(output != "str"){
            std::cout << 5 << std::endl;
            return 1;
        }
    } catch (BadShipOperationException& e) {
        std::cout << 6 << std::endl;
        return 1;
    }
    return 0;
}

int test9() {
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
        return 1;
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
        return 1;
    }

    try {
        auto output = ship->unload(X{1}, Y{1});
        if(output != "str"){
            std::cout << 5 << std::endl;
            return 1;
        }
    } catch (BadShipOperationException& e) {
        std::cout << 6 << std::endl;
        return 1;
    }
    return 0;
}

int test10() {
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
        return 0;

    return 1;
}

int test11() {
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
        return 0;

    return 1;
}

int test12() {
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
        return 0;

    return 1;
}

int test13() {
    Ship<std::string> ship {X{3}, Y{2}, Height{2}};

    ship.load(X{0}, Y{1}, "str");
    ship.load(X{0}, Y{1}, "sprr");
    ship.load(X{2}, Y{1}, "not_include");

    std::vector<std::string> v1 = {};

    auto view = ship.getContainersViewByPosition(X{10}, Y{1});

    std::vector<std::string> v2;
    for(const auto& container : view) {
        v2.push_back(container);
    }

    if(v1 == v2)
        return 0;

    return 1;
}

int test14() {
    Ship<std::string> ship {X{3}, Y{2}, Height{2}};

    ship.load(X{0}, Y{1}, "str");
    ship.load(X{0}, Y{1}, "sprr");
    ship.load(X{2}, Y{1}, "not_include");

    std::vector<std::string> v1 = {};

    auto view = ship.getContainersViewByPosition(X{10}, Y{11});

    std::vector<std::string> v2;
    for(const auto& container : view) {
        v2.push_back(container);
    }

    if(v1 == v2)
        return 0;

    return 1;
}

int test15() {
    Ship<std::string> ship {X{3}, Y{2}, Height{2}};

    ship.load(X{0}, Y{1}, "str");
    ship.load(X{0}, Y{1}, "sprr");
    ship.load(X{2}, Y{1}, "not_include");

    std::vector<std::string> v1 = {};

    auto view = ship.getContainersViewByPosition(X{0}, Y{11});

    std::vector<std::string> v2;
    for(const auto& container : view) {
        v2.push_back(container);
    }

    if(v1 == v2)
        return 0;

    return 1;
}

int test16() {
    Ship<std::string> ship {X{3}, Y{2}, Height{2}};

    ship.load(X{0}, Y{1}, "str");
    ship.load(X{0}, Y{1}, "sprr");
    ship.load(X{2}, Y{1}, "not_include");

    std::vector<std::string> v1 = {};

    auto view = ship.getContainersViewByPosition(X{0}, Y{0});

    std::vector<std::string> v2;
    for(const auto& container : view) {
        v2.push_back(container);
    }

    if(v1 == v2)
        return 0;

    return 1;
}

int test17() {
    Ship<std::string> ship {X{3}, Y{2}, Height{2}};

    ship.load(X{0}, Y{0}, "str");

    auto view0 = ship.getContainersViewByPosition(X{0}, Y{0});
    auto view1 = ship.getContainersViewByPosition(X{1}, Y{1});

    ship.move(X{0}, Y{0}, X{1}, Y{1});
    ship.load(X{0}, Y{0}, "sprr");


    std::vector<std::string> v11 = {"sprr"};

    std::vector<std::string> v21;
    for(const auto& container : view0) {
        v21.push_back(container);
    }

    if(v11 != v21)
        return 1;

    std::vector<std::string> v12 = {"str"};

    std::vector<std::string> v22;
    for(const auto& container : view1) {
        v22.push_back(container);
    }

    if(v12 != v22)
        return 1;

    return 0;
}

int test18() {
    Grouping<std::string> groupingFunctions = {
            { "first_letter",
                    [](const std::string& s){ return std::string(1, s[0]); }
            }
    };

    std::vector<std::tuple<X, Y, Height>> restrictions = {};

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2}, restrictions, groupingFunctions);

    ship->load(X{0}, Y{1}, "str");
    ship->load(X{0}, Y{1}, "str12");
    auto view_Hh = ship->getContainersViewByGroup("first_letter", "s");
    ship->unload(X{0}, Y{1});
    ship->load(X{1}, Y{1}, "sprr");

    std::vector<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> v1 =
            {std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{0}, Y{1}, Height{0} ,"str"),
             std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{1}, Y{1}, Height{0} ,"sprr")};

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
        return 0;

    return 1;
}

int test19() {
    Grouping<std::string> groupingFunctions = {
            { "first_letter",
                    [](const std::string& s){ return std::string(1, s[0]); }
            }
    };

    std::vector<std::tuple<X, Y, Height>> restrictions = {};

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2}, restrictions, groupingFunctions);

    ship->load(X{0}, Y{1}, "str");
    ship->load(X{0}, Y{1}, "str12");
    auto view_Hh = ship->getContainersViewByGroup("first_letter_not_real", "s");
    ship->unload(X{0}, Y{1});
    ship->load(X{1}, Y{1}, "sprr");

    std::vector<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> v1 = {};

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
        return 0;

    return 1;
}

int test20() {
    Grouping<std::string> groupingFunctions = {
            { "first_letter",
                    [](const std::string& s){ return std::string(1, s[0]); }
            }
    };

    std::vector<std::tuple<X, Y, Height>> restrictions = {};

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{2}, restrictions, groupingFunctions);

    ship->load(X{0}, Y{1}, "str");
    ship->load(X{0}, Y{1}, "str12");
    auto view_Hh = ship->getContainersViewByGroup("first_letter", "s");
    auto view_Hh2 = ship->getContainersViewByGroup("first_letter", "t");
    ship->load(X{1}, Y{1}, "tprr");
    std::vector<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> v2;
    int j=0;
    for(const auto& container_tuple : view_Hh) {
        (void)container_tuple;
        j++;
    }
    if(j!=2)
        return 1;
    int i=0;
    for(const auto& container_tuple : view_Hh2) {
        (void)container_tuple;
        i++;
    }
    if(i!=1)
        return 1;
    return 0;
}

int test21() {
    Grouping<std::string> groupingFunctions = {
            { "first_letter",
                    [](const std::string& s){ return std::string(1, s[0]); }
            }
    };

    std::vector<std::tuple<X, Y, Height>> restrictions = {
            std::tuple<X, Y, Height>(X{0}, Y{0}, Height{2}),
            std::tuple<X, Y, Height>(X{1}, Y{1}, Height{1}),
            std::tuple<X, Y, Height>(X{0}, Y{1}, Height{3}),
    };

    auto ship = new Ship<std::string>(X{3}, Y{2}, Height{4}, restrictions, groupingFunctions);

    ship->load(X{0}, Y{0}, "str1");
    ship->load(X{1}, Y{1}, "str2");
    ship->load(X{0}, Y{1}, "str3");
    ship->load(X{2}, Y{1}, "str4");

    std::vector<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> v1 =
            {std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{0}, Y{0}, Height{0} ,"str1"),
            std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{1}, Y{1}, Height{0} ,"str2"),
            std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{0}, Y{1}, Height{0} ,"str3"),
            std::tuple<shipping::X, shipping::Y, shipping::Height,std::string>(X{2}, Y{1}, Height{0} ,"str4")};


    auto view_Hh = ship->getContainersViewByGroup("first_letter", "s");

    std::vector<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> v2;
    for(const auto& container_tuple : view_Hh) {
        v2.emplace_back(
                std::get<0>(container_tuple.first), std::get<1>(container_tuple.first), std::get<2>(container_tuple.first) ,container_tuple.second);
    }

    std::set<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> s1;
    s1.insert(v1.begin(), v1.end());

    std::set<std::tuple<shipping::X, shipping::Y, shipping::Height, std::string>> s2;
    s2.insert(v2.begin(), v2.end());

//    for(auto s:s1){
//        std::cout << std::get<0>(s) << ", " << std::get<1>(s) << ", " << std::get<2>(s) << ", " << std::get<3>(s) << std::endl;
//    }
//
//    std::cout << " " << std::endl;
//
//
//    for(auto s:s2){
//        std::cout << std::get<0>(s) << ", " << std::get<1>(s) << ", " << std::get<2>(s) << ", " << std::get<3>(s) << std::endl;
//    }

    if(s1 == s2)
        return 0;

    return 1;
}


int main() {
    bool somethingFailed = false;
    if(test1()){
        somethingFailed = true;
        std::cout << "Test 1 failed" << std::endl;
    } else
        std::cout << "Test 1 passed" << std::endl;

    if(test2()){
        somethingFailed = true;
        std::cout << "Test 2 failed" << std::endl;
    } else
        std::cout << "Test 2 passed" << std::endl;

    if(test3()){
        somethingFailed = true;
        std::cout << "Test 3 failed" << std::endl;
    } else
        std::cout << "Test 3 passed" << std::endl;

    if(test4()){
        somethingFailed = true;
        std::cout << "Test 4 failed" << std::endl;
    } else
        std::cout << "Test 4 passed" << std::endl;

    if(test5()){
        somethingFailed = true;
        std::cout << "Test 5 failed" << std::endl;
    }else
        std::cout << "Test 5 passed" << std::endl;

    if(test6()){
        somethingFailed = true;
        std::cout << "Test 6 failed" << std::endl;
    } else
        std::cout << "Test 6 passed" << std::endl;

    if(test7()){
        somethingFailed = true;
        std::cout << "Test 7 failed" << std::endl;
    } else
        std::cout << "Test 7 passed" << std::endl;

    if(test8()){
        somethingFailed = true;
        std::cout << "Test 8 failed" << std::endl;
    } else
        std::cout << "Test 8 passed" << std::endl;

    if(test9()){
        somethingFailed = true;
        std::cout << "Test 9 failed" << std::endl;
    } else
        std::cout << "Test 9 passed" << std::endl;

    if(test10()){
        somethingFailed = true;
        std::cout << "Test 10 failed" << std::endl;
    } else
        std::cout << "Test 10 passed" << std::endl;

    if(test11()){
        somethingFailed = true;
        std::cout << "Test 11 failed" << std::endl;
    } else
        std::cout << "Test 11 passed" << std::endl;

    if(test12()){
        somethingFailed = true;
        std::cout << "Test 12 failed" << std::endl;
    } else
        std::cout << "Test 12 passed" << std::endl;

    if(test13()){
        somethingFailed = true;
        std::cout << "Test 13 failed" << std::endl;
    } else
        std::cout << "Test 13 passed" << std::endl;

    if(test14()){
        somethingFailed = true;
        std::cout << "Test 14 failed" << std::endl;
    } else
        std::cout << "Test 14 passed" << std::endl;

    if(test15()){
        somethingFailed = true;
        std::cout << "Test 15 failed" << std::endl;
    } else
        std::cout << "Test 15 passed" << std::endl;

    if(test16()){
        somethingFailed = true;
        std::cout << "Test 16 failed" << std::endl;
    } else
        std::cout << "Test 16 passed" << std::endl;

    if(test17()){
        somethingFailed = true;
        std::cout << "Test 17 failed" << std::endl;
    } else
        std::cout << "Test 17 passed" << std::endl;

    if(test18()){
        somethingFailed = true;
        std::cout << "Test 18 failed" << std::endl;
    } else
        std::cout << "Test 18 passed" << std::endl;

    if(test19()){
        somethingFailed = true;
        std::cout << "Test 19 failed" << std::endl;
    } else
        std::cout << "Test 19 passed" << std::endl;

    if(test20()){
        somethingFailed = true;
        std::cout << "Test 20 failed" << std::endl;
    } else
        std::cout << "Test 20 passed" << std::endl;

    if(test21()){
        somethingFailed = true;
        std::cout << "Test 21 failed" << std::endl;
    } else
        std::cout << "Test 21 passed" << std::endl;

    if(!somethingFailed) {
        std::cout << " " << std::endl;
        std::cout << "All Good!!" << std::endl;
    }
}
