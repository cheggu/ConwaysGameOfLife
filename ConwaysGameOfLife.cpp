#include "Includes.h"
#include "CreatureList.h"

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
                    if (timer.getElapsedTime().asMilliseconds() % 100 == 0) { //change the speed here!
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