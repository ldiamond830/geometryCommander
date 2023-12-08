#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "MyUtils.h"
template<class t>

class Button
{
private:
	sf::RectangleShape backGround;
	sf::Text label;
	void (t::* onClick)();
	t* parent;
	InputManager input;
	sf::Color defaultColor;
	sf::Color hoverColor;
public:
	Button(sf::Vector2f pos, sf::Vector2f size, sf::Text text, t* _parent, void(t::* _onClick)(), sf::Color _defaultColor = sf::Color::Black, sf::Color _hoverColor = sf::Color::White) {
		backGround.setPosition(pos);
		backGround.setFillColor(_defaultColor);
		backGround.setSize(size);
		defaultColor = _defaultColor;
		hoverColor = _hoverColor;
		backGround.setOrigin(backGround.getLocalBounds().width / 2.0f, backGround.getLocalBounds().height / 2.0f);
		label = text;
		MyUtils::GetInstance()->CenterText(&label);
		label.setPosition(backGround.getOrigin());
		parent = _parent;
		onClick = _onClick;
		input = InputManager();
	}

	void Update() {
		if (IsMouseOver) {
			//mouse in bounds and user has clicked
			if (input.isLeftMouseReleased()) {
				(parent->*onClick)();
			}
			//mouse is hovering over button
			else {
				backGround.setFillColor(hoverColor);
			}
		}
		else {
			//reset back from hover color
			backGround.setFillColor(defaultColor);
		}
		
	} 


	bool IsMouseOver() {
		sf::Vector2i mousePos = sf::Mouse::getPosition();

		if (backGround.getPosition().x > mousePos.x)
			return false;
		if (backGround.getPosition().x + backGround.getLocalBounds().width < mousePos.x)
			return false;
		if (backGround.getPosition().y > mousePos.y)
			return false;
		if (backGround.getPosition().y + backGround.getLocalBounds().height < mousePos.y)
			return false;

		return true;
	}

	void Draw(sf::RenderWindow* window) {
		window->draw(backGround);
		window->draw(label);
	}
};

