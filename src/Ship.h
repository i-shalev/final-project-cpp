//
// Created by itay on 08/06/2020.
//

#ifndef FINAL_PROJECT_CPP_SHIP_H
#define FINAL_PROJECT_CPP_SHIP_H

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <unordered_map>
#include <functional>
#include <list>

namespace shipping
{
    // helper classes for named arguments
    template<typename T>
    class Named {
        T value;
    public:
        explicit Named(T value): value{value} {}
        operator T() const {return value;}
    };

    class X: public Named<int> {
        using Named<int>::Named;
    };

    class Y: public Named<int> {
        using Named<int>::Named;
    };

    class Height: public Named<int> {
        using Named<int>::Named;
    };

    using Position = std::tuple<shipping::X, shipping::Y, shipping::Height>;
}

namespace std
{
    template<> struct hash<shipping::Position>
    {
        std::size_t operator()(const shipping::Position& pos) const noexcept
        {
            return std::get<0>(pos) ^ (std::get<1>(pos) << 1) ^ std::get<2>(pos);
        }
    };
}

namespace shipping
{
    int addOneIfNeeded = 1;

    class BadShipOperationException : public std::exception {
    public:
        explicit BadShipOperationException(std::string msg) {
            std::cout << msg << std::endl;
        }
    };

    template<typename Container>
    using Grouping = std::unordered_map<std::string, std::function<std::string(const Container&)>>;

    template<typename Container>
    class Ship{

        class GroupView {
            const std::unordered_map<Position, const Container&>* p_group = nullptr;
            using iterator_type = typename std::unordered_map<Position, const Container&>::const_iterator;
        public:
            GroupView(const std::unordered_map<Position, const Container&>& group): p_group(&group) {}
            GroupView(int) {}
            auto begin() const {
                return p_group? p_group->begin(): iterator_type{};
            }
            auto end() const {
                return p_group? p_group->end(): iterator_type{};
            }
        };

        public:
            Grouping<Container> groupingFunctions;
            std::vector<std::vector<std::vector<Container*>>> floors;
            std::vector<std::vector<std::vector<int>>> blocks;
            X x;
            Y y;
            Height height;
            using Pos2Container = std::unordered_map<Position, const Container&>;
            using Group = std::unordered_map<std::string, Pos2Container>;
            // all groupings by their grouping name
            std::unordered_map<std::string, Group> groups;

        public:
            Ship(X x, Y y, Height max_height) noexcept : height(max_height), x(x), y(y) {
                for(int xIndex = 0; xIndex < x; xIndex++){
                    floors.emplace_back();
                    blocks.emplace_back();
                    for(int yIndex = 0; yIndex < y; yIndex++){
                        floors.at(xIndex).emplace_back();
                        blocks.at(xIndex).emplace_back();
                        for(int hIndex = 0; hIndex < height; hIndex++){
                            floors.at(xIndex).at(yIndex).push_back(nullptr);
                            blocks.at(xIndex).at(yIndex).push_back(0);
                        }
                    }
                }
            }

            Ship(X x, Y y, Height max_height, std::vector<std::tuple<X, Y, Height>> restrictions) noexcept(false) : Ship(x, y, max_height) {
                for(auto tuple: restrictions){
                    int restrictionHeight = std::get<2>(tuple);
                    for(int i = 0; i < max_height - restrictionHeight; i++) {
                        blocks.at(std::get<0>(tuple)).at(std::get<1>(tuple)).at(i) = 1;
                    }
                }
            }

            Ship(X x, Y y, Height max_height, std::vector<std::tuple<X, Y, Height>> restrictions,
                 Grouping<Container> groupingFunctions) noexcept(false) : Ship(x, y, max_height, restrictions) {
                this->groupingFunctions = groupingFunctions;
            }

            void load(X xIndex, Y yIndex, Container c) noexcept(false) {
//                printf("Address inside is %p\n", (void *)&c);
                int heightIndexToInsert = findLastHeightIndex(xIndex, yIndex) + 1;
                if(heightIndexToInsert < 0 or heightIndexToInsert >= height){
                    throw BadShipOperationException("No place to insert the container.");
                }
                floors.at(xIndex).at(yIndex).at(heightIndexToInsert) = &c;
                addContainerToGroups(xIndex, yIndex, Height{heightIndexToInsert});
            }

            Container unload(X xIndex, Y yIndex) noexcept(false){
                int heightIndexOfContainer = findLastHeightIndex(xIndex, yIndex);
                if(heightIndexOfContainer < 0 or heightIndexOfContainer >= height){
                    throw BadShipOperationException("No container to unload from this position.");
                }
                if(floors.at(xIndex).at(yIndex).at(heightIndexOfContainer) == nullptr){
                    throw BadShipOperationException("No container to unload from this position.");
                }
                removeContainerFromGroups(xIndex, yIndex, Height{heightIndexOfContainer});
                auto container = *floors.at(xIndex).at(yIndex).at(heightIndexOfContainer);
                floors.at(xIndex).at(yIndex).at(heightIndexOfContainer) = nullptr;
                return container;
            }

            void move(X from_x, Y from_y, X to_x, Y to_y) noexcept(false) {
                if(from_x == to_x and from_y == to_y){
                    return;
                }
                int heightIndexOfContainer = findLastHeightIndex(from_x, from_y);
                if(heightIndexOfContainer < 0 or heightIndexOfContainer >= height){
                    throw BadShipOperationException("No container to move from this position.");
                }

                int heightIndexToInsert = findLastHeightIndex(to_x, to_y) + 1;
                if(heightIndexToInsert < 0 or heightIndexToInsert >= height ){
                    throw BadShipOperationException("No place to insert the container.");
                }

                floors.at(to_x).at(to_y).at(heightIndexToInsert) = floors.at(from_x).at(from_y).at(heightIndexOfContainer);
                addContainerToGroups(to_x, to_y, Height{heightIndexToInsert});
                removeContainerFromGroups(from_x, from_y, Height{heightIndexOfContainer});
                floors.at(from_x).at(from_y).at(heightIndexOfContainer) = nullptr;
            }

            int findLastHeightIndex(X xIndex, Y yIndex) noexcept {
                if(x < xIndex or y < yIndex){
                    return -2;
                }

                int currentIndex = 0;
                while(currentIndex < height){
                    if(blocks.at(xIndex).at(yIndex).at(currentIndex) == 1){
                        currentIndex++;
                        continue;
                    }
                    if(floors.at(xIndex).at(yIndex).at(currentIndex) == nullptr){
                        break;
                    }
                    currentIndex++;
                }
                return currentIndex-1;
            }

            void addContainerToGroups(X x, Y y, int h) {
                Container& e = *(floors.at(x).at(y).at(h));
                for(auto& group_pair: groupingFunctions) {
                    groups[group_pair.first][group_pair.second(e)].insert( { std::tuple<X,Y,Height>{x, y, h}, e } );
                }
            }

            void removeContainerFromGroups(X x, Y y, int h) {
                Container& e = *(floors.at(x).at(y).at(h));
                for(auto& group_pair: groupingFunctions) {
                    groups[group_pair.first][group_pair.second(e)].erase(std::tuple<X,Y,Height>{x, y,  h});
                }
            }

            GroupView getContainersViewByGroup(const std::string& groupingName, const std::string& groupName) const {
                auto itr = groups.find(groupingName);
                if(itr != groups.end()) {
                    const auto& grouping = itr->second;
                    auto itr2 = grouping.find(groupName);
                    if(itr2 != grouping.end()) {
                        return GroupView { groups.at(groupingName).at(groupName) };
                    }
                }
                return GroupView { 0 };
            }

    };
};

#endif //FINAL_PROJECT_CPP_SHIP_H
