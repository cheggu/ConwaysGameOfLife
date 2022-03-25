#pragma once
#include "Includes.h"

class CreatureList {
private:
    std::pair<std::pair<int, int>, sf::RectangleShape>& searchPairByXY(int x, int y) {
        for (std::pair<std::pair<int, int>, sf::RectangleShape>& pair : list) {
            //std::cout << "X" + std::to_string(pair.first.first) + " Y" + std::to_string(pair.first.second) + "\n";
            if (pair.first.first == x && pair.first.second == y) {
                return pair;
            }
        }
        std::pair<std::pair<int, int>, sf::RectangleShape> foo = { {-1, -1}, sf::RectangleShape({-1,-1}) };
        return foo;
    }

    void kill(std::pair<int, int> creature) {
        int i = 0;

        while (i < aliveList.size())
            if (aliveList.size() != 0) {
                if (aliveList[i].first == creature.first && aliveList[i].second == creature.second) {
                    //std::cout << "found: " << creature.first << " " << creature.second << std::endl;
                    aliveList.erase(aliveList.begin() + i);
                }
                else {
                    i++;
                }
            }
    }

    const int leftPadding = 200, topPadding = 10, rightPadding = 10, bottomPadding = 10;

public:
    std::vector<std::pair<std::pair<int, int>, sf::RectangleShape>> list;
    std::vector<std::pair<int, int>> aliveList = { };

    const int width = (WIDTH - rightPadding - leftPadding) / 10;
    const int height = (HEIGHT - bottomPadding - topPadding) / 10;

    void populate() {
        list.clear();
        sf::RectangleShape rect;
        rect.setFillColor(sf::Color::Black);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(1);
        rect.setSize({ 10,10 });

        for (int i = 0 + leftPadding; i < WIDTH - rightPadding; i += 10) {
            for (int o = 0 + topPadding; o < HEIGHT - bottomPadding; o += 10) {
                rect.setPosition(i, o);
                list.push_back({ {i, o}, rect });
            }
        }
    }

    void simulate() {
        for (std::pair<std::pair<int, int>, sf::RectangleShape> pair : list) {
            int aliveNeighboors = getAliveNeighboorCount(pair.first);

            if (isAlive(pair.first)) {

                if (aliveNeighboors == 2 || aliveNeighboors == 3) {
                    //stay alive
                }
                else {
                    kill(pair.first);
                }
                //std::cout << "X" << pair.first.first << "Y" << pair.first.second << " cnt: " << aliveNeighboors << "\n";
            }
            else { //creature is dead
                if (aliveNeighboors == 3) {
                    aliveList.push_back(pair.first);
                }
            }
        }
    }

    std::vector<std::pair<int, int>> findNeighbours(int x, int y) {
        return { {x - 10, y}, {x + 10, y}, {x, y - 10}, {x, y + 10}, {x - 10, y - 10}, {x - 10, y + 10}, {x + 10, y - 10}, {x + 10, y + 10} };
    }

    int getAliveNeighboorCount(std::pair<int, int> coord) {
        int alive = 0;

        std::vector<std::pair<int, int>> neighboors = findNeighbours(coord.first, coord.second);
        for (std::pair<int, int> neighboor : neighboors) {
            if (isAlive(neighboor)) {
                if (neighboor.first > leftPadding && neighboor.second > topPadding && neighboor.first < WIDTH - rightPadding && neighboor.second < HEIGHT - bottomPadding) {
                    alive++;
                }
            }
        }
        //std::cout << "X" << coord.first << "Y" << coord.second << " cnt: " << alive << "\n";
        return alive;
    }

    bool isAlive(std::pair<int, int> creature) {
        if (std::find(aliveList.begin(), aliveList.end(), creature) != aliveList.end()) {
            return true;
        }
        return false;
    }

    void removeDupes() {
        std::sort(aliveList.begin(), aliveList.end());
        aliveList.erase(std::unique(aliveList.begin(), aliveList.end()), aliveList.end());
    }

    sf::RectangleShape* searchShapeByXY(int x, int y) {
        return &searchPairByXY(x * 10, y * 10).second;
    }

    void searchAndSetPosXY(int searchX, int searchY, float posX, float posY) {
        searchShapeByXY(searchX * 10, searchY * 10)->setPosition(posX, posY);
    }
};