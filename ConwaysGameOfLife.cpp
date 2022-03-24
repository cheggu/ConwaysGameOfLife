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

//sf::RectangleShape& getRectFromList(std::vector<sf::RectangleShape> list, int x, int y) {
    
//}

struct compareXY {
    bool operator()(const sf::Vector2f* a, const sf::Vector2f* b) const
    {
        return a->x < b->x && a->y < b->y;
    }
};
 
void populateCreatureList(std::map<sf::Vector2f*, sf::RectangleShape, compareXY>& list) {
    list.clear();
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(1);
    rect.setSize({ 10,10 });

    for (int i = 0 + 200; i < WIDTH - 10; i+=10) {
        for (int o = 0 + 10; o < HEIGHT - 10; o+=10) {
            rect.setPosition(i, o);

            //list.insert(std::map<int, std::pair<sf::Vec

            list.insert(std::pair<sf::Vector2f*, sf::RectangleShape>(new sf::Vector2f{ float(o), float(o) }, rect) );
        }
    }
    
}

int main()
{
    int width = WIDTH;
    int height = HEIGHT;

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
    stats.setString("width: " + std::to_string(WIDTH) + "\nheight: " + std::to_string(HEIGHT));
    stats.setCharacterSize(16); // in pixels, not points!
    stats.setFillColor(sf::Color::Red);
    stats.setPosition(10, 30);
    stats.setStyle(sf::Text::Bold | sf::Text::Underlined);

    std::map<sf::Vector2f*, sf::RectangleShape, compareXY> creatureList;
    populateCreatureList(creatureList);

    while (window.isOpen())
    {
        window.clear();

        dt = dt_clock.restart().asSeconds();

        if ((int)timer.getElapsedTime().asMilliseconds() % 1000 == 0) {
            stats.setString("width: " + std::to_string(WIDTH) + "\nheight: " + std::to_string(HEIGHT) + "\nfps: " + std::to_string((int)(1 / dt))
                            + "\ncreatureList size: " + std::to_string(creatureList.size()));
        }
        else {

        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (std::pair<sf::Vector2f*, sf::RectangleShape> pair: creatureList) {
            window.draw(pair.second);
        }
        window.draw(stats);
        window.draw(time);

        window.display();
    }
}