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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		mostRecentKey = sf::Keyboard::Enter;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		mostRecentKey = sf::Keyboard::Up;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		mostRecentKey = sf::Keyboard::Down;
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

bool InputManager::isLeftMouseReleased() {
	if (!mouseBeingClicked) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			mouseBeingClicked = true;
		}
	}
	else {
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			mouseBeingClicked = false;
			return true;
		}
	}

	return false;
}
