#include "AudioManager.h"
AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager() {
	MoveSound.loadFromFile("sounds/MoveSound");
}

AudioManager::~AudioManager()
{
	delete instance;
}

AudioManager* AudioManager::GetInstance()
{
	if (instance == nullptr) 
	{
		instance = new AudioManager();
    }

	return instance;
}

void AudioManager::PlayMoveSound()
{
	output.setBuffer(MoveSound);
	output.play();
}


