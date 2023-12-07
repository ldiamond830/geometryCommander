#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class InputManager
{
private:
	sf::Keyboard::Key mostRecentKey;
	bool mouseBeingClicked;
public:
	void PlayerInput();
	bool isKeyReleased(sf::Keyboard::Key);
	bool isLeftMouseReleased();
};

