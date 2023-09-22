#include "InputManager.h"

void InputManager::PlayerInput()
{
	//only keys currently in use
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		mostRecentKey = sf::Keyboard::Q;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		mostRecentKey = sf::Keyboard::E;
	}
}

bool InputManager::isKeyReleased(sf::Keyboard::Key keyToCheck)
{
	if (mostRecentKey == keyToCheck) {
		if (!sf::Keyboard::isKeyPressed(keyToCheck)) {
			mostRecentKey = sf::Keyboard::Unknown;
			return true;
		}
    }
	return false;
}
