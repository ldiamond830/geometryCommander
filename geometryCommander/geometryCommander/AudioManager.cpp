#include "AudioManager.h"
#include <iostream>
AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager() {
	if (!moveSound.loadFromFile("sounds/move.wav")) {
		std::cout << "error loading move sound";
	}
	if (!invalidInputSound.loadFromFile("sounds/invalidInput.wav")) {
		std::cout << "error loading invalid input sound";
	}
	if (!dieSound.loadFromFile("sounds/die.wav")) {
		std::cout << "error loading die sound";
	}
}

AudioManager* AudioManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new AudioManager();
	}

	return instance;
}

AudioManager::~AudioManager()
{
}

void AudioManager::PlayMoveSound()
{
	if (playback.getStatus() != sf::SoundSource::Status::Playing) {
		playback.setBuffer(moveSound);
		playback.play();
	}
}

void AudioManager::PlayInvalidInputSound()
{
	if (playback.getStatus() != sf::SoundSource::Status::Playing) {
		playback.setBuffer(invalidInputSound);
		playback.play();
	}	
}

void AudioManager::PlayDieSound()
{
	if (playback.getStatus() != sf::SoundSource::Status::Playing) {
		playback.setBuffer(dieSound);
		playback.play();
	}
}
