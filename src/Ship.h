//itayshalev 206283228

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
#include <set>

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
    class BadShipOperationException : public std::exception {
        std::string msg;
    public:
        BadShipOperationException(std::string msg) : msg(msg) {
        }

        std::string getMessage() {
            return msg;
        }
    };

    template<typename Container>
    using Grouping = std::unordered_map<std::string, std::function<std::string(const Container&)>>;

    template<typename Container>
    class Ship{

        class const_iterator {
            X maxX;
            Y maxY;
            Height maxH;
            X currentX;
            Y currentY;
            Height currentH;
            std::vector<std::vector<std::vector<Container>>>& floors;
            std::vector<std::vector<std::vector<int>>>& valid;

        public:
            const_iterator(X currentX, Y currentY, Height currentH, std::vector<std::vector<std::vector<Container>>>& floors, std::vector<std::vector<std::vector<int>>>& valid,  X maxX,  Y maxY,  Height maxH) :
                    maxX(maxX), maxY(maxY), maxH(maxH), currentX(currentX), currentY(currentY), currentH(currentH), floors(floors), valid(valid) {
                if(!(currentX == -1 and currentY == -1 and currentH == -1)) {
                    if(valid.at(currentX).at(currentY).at(currentH) == 0)
                        findNextContainer();
                }
            }
            const_iterator operator++() {
                findNextContainer();
                return *this;
            }

            const Container& operator*() const {
                return floors.at(currentX).at(currentY).at(currentH);
            }

            bool operator!=(const_iterator itr) const {
                return currentX != itr.currentX or currentY != itr.currentY or currentH != itr.currentH or &floors != &itr.floors;
            }

            void findNextContainer() {
                if(currentX == -1 and currentY == -1 and currentH == -1)
                    return;
                nextIndex();
                if(currentX == -1 and currentY == -1 and currentH == -1)
                    return;
                while(valid.at(currentX).at(currentY).at(currentH) == 0) {
                    nextIndex();
                    if(currentX == -1 and currentY == -1 and currentH == -1)
                        return;
                }
            }

            void nextIndex() {
                if(currentH < maxH - 1) {
                    currentH = Height{currentH+1};
                    return;
                }
                if(currentY < maxY - 1) {
                    currentH = Height{0};
                    currentY = Y{currentY+1};
                    return;
                }
                if(currentX < maxX -1) {
                    currentH = Height{0};
                    currentY = Y{0};
                    currentX = X{currentX+1};
                    return;
                }
                currentH = Height{-1};
                currentY = Y{-1};
                currentX = X{-1};
            }
        };

        class position_const_iterator {
            X currentX;
            Y currentY;
            Height currentH;
            const std::vector<std::vector<std::vector<Container>>>& floors;
            const std::vector<std::vector<std::vector<int>>>& valid;

        public:
            position_const_iterator(X currentX, Y currentY, Height currentH, const std::vector<std::vector<std::vector<Container>>>& floors, const std::vector<std::vector<std::vector<int>>>& valid) :
                    currentX(currentX), currentY(currentY), currentH(currentH), floors(floors), valid(valid) {
                if(currentH != -1) {
                    if(valid.at(currentX).at(currentY).at(currentH) == 0)
                        findNextContainer();
                }
            }

            position_const_iterator operator++() {
                findNextContainer();
                return *this;
            }

            const Container& operator*() const {
                return floors.at(currentX).at(currentY).at(currentH);
            }

            bool operator!=(position_const_iterator itr) const {
                return currentX != itr.currentX or currentY != itr.currentY or currentH != itr.currentH or &floors != &itr.floors;
            }

            void findNextContainer() {
                if(currentH == -1)
                    return;
                nextIndex();
                if(currentH == -1)
                    return;
                while(valid.at(currentX).at(currentY).at(currentH) == 0) {
                    nextIndex();
                    if(currentH == -1)
                        return;
                }
            }

            void nextIndex() {
                currentH = Height{currentH-1};
            }
        };

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

        class viewByPosition {
            X x;
            Y y;
            Height maxHeight;
            const std::vector<std::vector<std::vector<Container>>>& floors;
            const std::vector<std::vector<std::vector<int>>>& valid;
            bool validIndex;

        public:
            viewByPosition(X x, Y y, Height maxHeight, const std::vector<std::vector<std::vector<Container>>>& floors, const std::vector<std::vector<std::vector<int>>>& valid, bool validIndex):
                x(x), y(y), maxHeight(maxHeight), floors(floors), valid(valid), validIndex(validIndex) {}
            auto begin() const{
                if(validIndex)
                    return position_const_iterator(x, y, Height{maxHeight-1}, floors, valid);
                return position_const_iterator(x, y, Height{-1}, floors, valid);
            }
            auto end() const{
                return position_const_iterator(x, y, Height{-1}, floors, valid);
            }
        };

        public:
            Grouping<Container> groupingFunctions;
            std::vector<std::vector<std::vector<Container>>> floors;
            std::vector<std::vector<std::vector<int>>> valid;
            std::vector<std::vector<std::vector<int>>> blocks;
            X x;
            Y y;
            Height height;
            using Pos2Container = std::unordered_map<Position, const Container&>;
            using Group = std::unordered_map<std::string, Pos2Container>;
            // all groupings by their grouping name
            // mutable as this is not part of the state of the class
            // it is required in order to allow getView to create non-existing groups
            mutable std::unordered_map<std::string, Group> groups;

        public:
            Ship(X x, Y y, Height max_height) noexcept :  x(x), y(y), height(max_height) {
                for(int xIndex = 0; xIndex < x; xIndex++){
                    floors.emplace_back();
                    valid.emplace_back();
                    blocks.emplace_back();
                    for(int yIndex = 0; yIndex < y; yIndex++){
                        floors.at(xIndex).emplace_back();
                        valid.at(xIndex).emplace_back();
                        blocks.at(xIndex).emplace_back();
                        for(int hIndex = 0; hIndex < height; hIndex++){
                            floors.at(xIndex).at(yIndex).emplace_back();
                            valid.at(xIndex).at(yIndex).push_back(0);
                            blocks.at(xIndex).at(yIndex).push_back(0);
                        }
                    }
                }
            }

            Ship(X x, Y y, Height max_height, std::vector<std::tuple<X, Y, Height>> restrictions) noexcept(false) : Ship(x, y, max_height) {
                std::set<std::tuple<X, Y>> set;
                for(auto tuple: restrictions){
                    auto xIndex = std::get<0>(tuple);
                    auto yIndex = std::get<1>(tuple);
                    checkParams(xIndex, yIndex);
                    if(set.find(std::tuple<X, Y>(xIndex, yIndex)) != set.end()) {
                        throw BadShipOperationException("Duplicate restrictions (whether or not it has same limit).");
                    }
                    set.insert(std::tuple<X, Y>(xIndex, yIndex));
                    int restrictionHeight = std::get<2>(tuple);
                    if(restrictionHeight >= height) {
                        throw BadShipOperationException("Restriction with height equal or greater than the original height.");
                    }
                    for(int i = 0; i < max_height - restrictionHeight; i++) {
                        blocks.at(std::get<0>(tuple)).at(std::get<1>(tuple)).at(i) = 1;
                    }
                }
            }

            Ship(X x, Y y, Height max_height, std::vector<std::tuple<X, Y, Height>> restrictions,
                 Grouping<Container> groupingFunctions) noexcept(false) : Ship(x, y, max_height, restrictions) {
                this->groupingFunctions = groupingFunctions;
            }

            void checkParams(X xIndex, Y yIndex) noexcept(false) {
                if(xIndex >= x or xIndex < 0 ){
                    throw BadShipOperationException("X index out of range.");
                }
                if(yIndex >= y or yIndex < 0) {
                    throw BadShipOperationException("Y index out of range.");
                }
            }

            void load(X x, Y y, Container c) noexcept(false) {
                checkParams(x, y);
                int heightIndexToInsert = findLastHeightIndex(x, y) + 1;
                if(heightIndexToInsert < 0 or heightIndexToInsert >= height){
                    throw BadShipOperationException("No place to insert the container.");
                }
                floors.at(x).at(y).at(heightIndexToInsert) = c;
                valid.at(x).at(y).at(heightIndexToInsert) = 1;
                addContainerToGroups(x, y, Height{heightIndexToInsert});
            }

            Container unload(X x, Y y) noexcept(false){
                checkParams(x, y);
                int heightIndexOfContainer = findLastHeightIndex(x, y);
                if(heightIndexOfContainer < 0 or heightIndexOfContainer >= height){
                    throw BadShipOperationException("No container to unload from this position.");
                }
                if(valid.at(x).at(y).at(heightIndexOfContainer) == 0){
                    throw BadShipOperationException("No container to unload from this position.");
                }
                removeContainerFromGroups(x, y, Height{heightIndexOfContainer});
                auto container = floors.at(x).at(y).at(heightIndexOfContainer);
                valid.at(x).at(y).at(heightIndexOfContainer) = 0;
                return container;
            }

            void move(X from_x, Y from_y, X to_x, Y to_y) noexcept(false) {
                checkParams(from_x, from_y);
                checkParams(to_x, to_y);

                int heightIndexOfContainer = findLastHeightIndex(from_x, from_y);
                if(heightIndexOfContainer < 0 or heightIndexOfContainer >= height){
                    throw BadShipOperationException("No container to move from this position.");
                }

                if(from_x == to_x and from_y == to_y){
                    return;
                }


                int heightIndexToInsert = findLastHeightIndex(to_x, to_y) + 1;
                if(heightIndexToInsert < 0 or heightIndexToInsert >= height ){
                    throw BadShipOperationException("No place to insert the container.");
                }

                floors.at(to_x).at(to_y).at(heightIndexToInsert) = floors.at(from_x).at(from_y).at(heightIndexOfContainer);
                addContainerToGroups(to_x, to_y, Height{heightIndexToInsert});
                removeContainerFromGroups(from_x, from_y, Height{heightIndexOfContainer});
                valid.at(to_x).at(to_y).at(heightIndexToInsert) = 1;
                valid.at(from_x).at(from_y).at(heightIndexOfContainer) = 0;
            }

            int findLastHeightIndex(X xIndex, Y yIndex) noexcept {
                if(x < xIndex or y < yIndex or xIndex < 0 or yIndex < 0){
                    return -2;
                }

                int currentIndex = 0;
                while(currentIndex < height){
                    if(blocks.at(xIndex).at(yIndex).at(currentIndex) == 1){
                        currentIndex++;
                        continue;
                    }
                    if(valid.at(xIndex).at(yIndex).at(currentIndex) == 0){
                        break;
                    }
                    currentIndex++;
                }
                return currentIndex-1;
            }

            void addContainerToGroups(X x, Y y, Height h) {
                Container& e = floors.at(x).at(y).at(h);
                int numBlocks = 0;
                for(int i = 0; i < h; i++) {
                    if(blocks.at(x).at(y).at(i) == 1) {
                        numBlocks++;
                    } else {
                        break;
                    }
                }
                Height newH = Height{h - numBlocks};
                for(auto& group_pair: groupingFunctions) {
                    groups[group_pair.first][group_pair.second(e)].insert( { std::tuple<X,Y,Height>{x, y, newH}, e } );
                }
            }

            void removeContainerFromGroups(X x, Y y, int h) {
                Container& e = floors.at(x).at(y).at(h);
                int numBlocks = 0;
                for(int i = 0; i < h; i++) {
                    if(blocks.at(x).at(y).at(i) == 1) {
                        numBlocks++;
                    } else {
                        break;
                    }
                }
                Height newH = Height{h - numBlocks};
                for(auto& group_pair: groupingFunctions) {
                    groups[group_pair.first][group_pair.second(e)].erase(std::tuple<X,Y,Height>{x, y,  newH});
                }
            }

            GroupView getContainersViewByGroup(const std::string& groupingName, const std::string& groupName) const {
                auto itr = groups.find(groupingName);
                if(itr == groups.end() && groupingFunctions.find(groupingName) != groupingFunctions.end()) {
                    auto pair = groups.insert({groupingName, Group{}});
                    itr = pair.first;
                }
                if(itr != groups.end()) {
                    const auto& grouping = itr->second;
                    auto itr2 = grouping.find(groupName);
                    if(itr2 == grouping.end()) {
                        auto pair = itr->second.insert({groupName, Pos2Container{}});
                        itr2 = pair.first;
                    }
                    return GroupView { itr2->second };

                }
                return GroupView { 0 };
            }

            viewByPosition getContainersViewByPosition(X x, Y y) const{
                if(x < this->x and y < this->y and x >= 0 and y >= 0)
                    return viewByPosition(x, y, height, floors, valid, true);
                return viewByPosition(x, y, height, floors, valid, false);
            }

            const_iterator begin() {
                return const_iterator(X{0}, Y{0}, Height{0}, floors, valid, x, y, height);
            }

            const_iterator end() {
                return const_iterator(X{-1}, Y{-1}, Height{-1}, floors, valid, x, y, height);
            }

        // deleted because https://moodle.tau.ac.il/mod/forum/discuss.php?d=136637
        Ship(const Ship&) = delete;
        Ship& operator=(const Ship&) = delete;
        Ship(Ship&&) = delete;
        Ship& operator=(Ship&&) = delete;
    };
}

#endif //FINAL_PROJECT_CPP_SHIP_H
