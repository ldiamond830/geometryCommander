#include "InputManager.h"

void InputManager::PlayerInput()
{
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
	if (!leftMouseBeingClicked) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			leftMouseBeingClicked = true;
		}
	}
	else {
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			leftMouseBeingClicked = false;
			return true;
		}
	}

	return false;
}

bool InputManager::isRightMouseReleased()
{
	if (!rightMouseBeingClicked) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			rightMouseBeingClicked = true;
		}
	}
	else {
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			rightMouseBeingClicked = false;
			return true;
		}
	}

	return false;
}
