// geometryCommander.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "AppManager.h"
#include "GameManager.h"
using namespace std;

void wrapper();

int main()
{
    //uses wrapper function to avoid false positive memory leaks 
    wrapper();

    return 0;
}

void wrapper(){
    srand(time(NULL));
    int screenWidth = 800;
    int screenHeight = 600;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Game");
    AppManager* appManager = AppManager::CreateInstance(&window, screenWidth, screenHeight);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        appManager->Update();

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

        appManager->Draw();
        // end the current frame

        window.display();
    }

    //clear memory at the end of the game instance
    AppManager::DeleteInstance();
}


