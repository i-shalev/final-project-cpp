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

namespace shipping
{
    int addOneIfNeeded = 1;

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

    class BadShipOperationException : public std::exception {
        const std::string msg;

    public:
        explicit BadShipOperationException(std::string msg) : msg(msg) {}

        std::string getMessage() { return msg; }
    };

    template<typename Container>
    using Grouping = std::unordered_map<std::string, std::function<std::string(const Container&)>>;

    template<typename Container>
    class Ship{

        public:
            Grouping<Container> groupingFunctions;
            std::vector<std::vector<std::vector<Container*>>> floors;
            std::vector<std::vector<std::vector<int>>> blocks;
            X x;
            Y y;
            Height height;
//            std::vector<Container&> allContainers;

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
                int heightIndexToInsert = findLastHeightIndex(xIndex, yIndex) + 1;
                if(heightIndexToInsert < 0 or heightIndexToInsert >= height){
                    throw BadShipOperationException("No place to insert the container.");
                }
                floors.at(xIndex).at(yIndex).at(heightIndexToInsert) = &c;
//                allContainers.push_back(c);
            }

            Container unload(X xIndex, Y yIndex) noexcept(false){
                int heightIndexOfContainer = findLastHeightIndex(xIndex, yIndex);
                if(heightIndexOfContainer < 0 or heightIndexOfContainer >= height){
                    throw BadShipOperationException("No container to unload from this position.");
                }
                if(floors.at(xIndex).at(yIndex).at(heightIndexOfContainer) == nullptr){
                    throw BadShipOperationException("No container to unload from this position.");
                }
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

//            // iteration over factories
//            typedef typename std::vector<Container>::const_iterator const_iterator;
//
//            // Returns an iterator to the first algorithm factory
//            inline const_iterator begin() const { return allContainers.begin(); }
//
//            // Returns an iterator that points to the end
//            inline const_iterator end() const { return allContainers.end(); }

    };
};

#endif //FINAL_PROJECT_CPP_SHIP_H
