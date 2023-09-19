// geometryCommander.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "EnemyPiece.h"
#include "GridBox.h"
#include "PlayerPiece.h"
#include <vector>
#include <memory>
using namespace std;

void PlayerInput(Grid*, PlayerPiece*, sf::Window*);


int main()
{
    srand(time(NULL));
    int screenWidth = 800;
    int screenHeight = 600;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Game");
    Grid grid = Grid(screenWidth, screenHeight, 10, 10);
    
   

    vector<EnemyPiece*> enemyList;
    vector<PlayerPiece*> playerPieceList;
    //testing
    {
        GridBox* spawnPoint = grid.gridBoxes[0][0];
        EnemyPiece* enemy = new EnemyPiece(spawnPoint->GetCenter().x, spawnPoint->GetCenter().y);
        enemyList.push_back(enemy);
        spawnPoint->occupyingPiece = enemy;
        

        spawnPoint = grid.gridBoxes[0][1];
        PlayerPiece* playerPiece = new PlayerPiece(spawnPoint->GetCenter().x, spawnPoint->GetCenter().y);
        playerPieceList.push_back(playerPiece);
        spawnPoint->occupyingPiece = playerPiece;

        grid.MovePiece(spawnPoint, grid.gridBoxes[4][4]);
    }


    
    // run the program as long as the window is open
    while (window.isOpen())
    {
        //update
        {
            PlayerInput(&grid, playerPieceList[0], &window);
        }

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
        grid.Draw(&window);
        for (EnemyPiece* enemy : enemyList) {
            enemy->Draw(&window);
        }
        for (PlayerPiece* playerPiece : playerPieceList) {
            playerPiece->Draw(&window);
        }
        // end the current frame
        window.display();
    }

    for (EnemyPiece* enemy : enemyList) {
        delete enemy;
    }

    return 0;
}

void PlayerInput(Grid* grid, PlayerPiece* selectedPiece, sf::Window* window) 
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
       auto clickedBox = grid->GetBoxFromPosition(sf::Mouse::getPosition(*window));
       if (clickedBox != nullptr) {
           if (clickedBox->GetType() == gridBoxType::empty) {
               grid->MovePiece(grid->GetBoxFromPosition(selectedPiece->GetPosition()), clickedBox);
           }
       }
    }
}


