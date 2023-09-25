// geometryCommander.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "GameManager.h"
using namespace std;

void wrapper();

int main()
{
    wrapper();

    if (_CrtDumpMemoryLeaks()) {
         cout << "memory leak";
    }
    return 0;
}

void wrapper(){
    //EnemyPiece* enemyLeakTest = new EnemyPiece(0,0);
    //PlayerPiece* playerLeakTest = new PlayerPiece(0, 0);
    sf::RenderWindow window(sf::VideoMode(1, 1), "Game");
    GameManager* gameManagerLeakTest = new GameManager(&window, 1, 1, 10, 10, 4, 4);

    /*
    srand(time(NULL));
    int screenWidth = 800;
    int screenHeight = 600;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Game");
    GameManager gameManger = GameManager(&window, screenWidth, screenHeight, 10, 10, 4, 4);


    // run the program as long as the window is open
    while (window.isOpen())
    {
        gameManger.Update();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        gameManger.Draw();
        // end the current frame
        window.display();
    }
    */

    //delete enemyLeakTest;
    //delete playerLeakTest;
    delete gameManagerLeakTest;
    //delete &window;
}


