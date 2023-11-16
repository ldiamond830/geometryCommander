#include "AudioManager.h"
#include <iostream>
AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager() {
	if (!moveSound.loadFromFile("sounds/testing.wav")) {
		std::cout << "error loading move sound";
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
	playback.setBuffer(moveSound);
	playback.play();
}
