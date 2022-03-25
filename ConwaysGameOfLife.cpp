// ConwaysGameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
Any live cell with fewer than two live neighbours dies, as if by underpopulation.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
These rules, which compare the behavior of the automaton to real life, can be condensed into the following:

Any live cell with two or three live neighbours survives.
Any dead cell with three live neighbours becomes a live cell.
All other live cells die in the next generation. Similarly, all other dead cells stay dead.
*/

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
        /*
        Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            Any live cell with two or three live neighbours lives on to the next generation.
            Any live cell with more than three live neighbours dies, as if by overpopulation.
            Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            These rules, which compare the behavior of the automaton to real life, can be condensed into the following :

        Any live cell with two or three live neighbours survives.
            Any dead cell with three live neighbours becomes a live cell.
            All other live cells die in the next generation.Similarly, all other dead cells stay dead.
            */

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

        std::vector<std::pair<int,int>> neighboors = findNeighbours(coord.first, coord.second);
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

bool IsOdd(int i) {
    return ((i % 2) == 1);
}

int main()
{
    int width = WIDTH;
    int height = HEIGHT;

    bool lockClick = false;
    bool pause = false;

    CreatureList creatureList;
    creatureList.populate();

    float dt;
    sf::Clock dt_clock;

    sf::Clock timer;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        // error...
    }
    sf::Text time;
    time.setFont(font);
    time.setString("Elapsed: " + std::to_string(timer.getElapsedTime().asSeconds()));
    time.setCharacterSize(16); // in pixels, not points!
    time.setFillColor(sf::Color::Red);
    time.setPosition(10, 10);
    time.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text stats;
    stats.setFont(font);
    stats.setString("width: " + std::to_string(WIDTH) + "px\nor " + std::to_string(creatureList.width) + " blocks\nheight : " + std::to_string(HEIGHT) + "px\nor " + std::to_string(creatureList.width) + " blocks");
    stats.setCharacterSize(16); // in pixels, not points!
    stats.setFillColor(sf::Color::Red);
    stats.setPosition(10, 30);
    stats.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Press 'R' to Reset\nPress 'P' to Pause");
    instructions.setCharacterSize(16); // in pixels, not points!
    instructions.setFillColor(sf::Color::Red);
    instructions.setPosition(10, 160);
    instructions.setStyle(sf::Text::Bold | sf::Text::Underlined);

    while (window.isOpen())
    {
        window.clear();

        dt = dt_clock.restart().asSeconds();

        if ((int)timer.getElapsedTime().asMilliseconds() % 100 == 0) {
            stats.setString("width: " + std::to_string(WIDTH) + "px\nor " + std::to_string(creatureList.width) + " blocks\nheight : " + std::to_string(HEIGHT) + "px\nor " + std::to_string(creatureList.width) + " blocks\nfps : " + std::to_string((int)(1 / dt))
                            + "\ncreatureList size: " + std::to_string(creatureList.list.size())
                            + "\naliveList size: " + std::to_string(creatureList.aliveList.size()));
        }
        time.setString("Elapsed: " + std::to_string(timer.getElapsedTime().asSeconds()));

        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            creatureList.aliveList.clear();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            if (pause) {
                pause = false;
                instructions.setString("Press 'R' to Reset\nPress 'P' to Pause");
            }
            else {
                pause = true;
                instructions.setString("Press 'R' to Reset\nPress 'P' to Pause\nPAUSED");
            }
        }
        
        

        for (std::pair<std::pair<int, int>, sf::RectangleShape> pair : creatureList.list) {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && lockClick != true) {
                
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                if (pos.x - (pos.x % 10) == pair.first.first && pos.y - (pos.y % 10) == pair.first.second) {
                    creatureList.aliveList.push_back({ pair.first.first, pair.first.second });
                   
                }
            }
            
            if (!pause) {
                {
                    if (timer.getElapsedTime().asMilliseconds() % 100 == 0) {
                        creatureList.simulate();
                    }
                }
            }

            if (creatureList.isAlive(pair.first))
            {   
                pair.second.setFillColor(sf::Color::Red);
            }
            
            window.draw(pair.second);
        }


        creatureList.removeDupes();
        
        
        window.draw(stats);
        window.draw(time);
        window.draw(instructions);

        window.display();

        
    }
}