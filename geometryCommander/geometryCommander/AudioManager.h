#pragma once
#define SFML_STATIC
#include <SFML/Audio.hpp>
class AudioManager
{
private:
	sf::Sound playback;
	sf::SoundBuffer moveSound;
	AudioManager();
	static AudioManager* instance;
public:
	static AudioManager* GetInstance();
	~AudioManager();
	void PlayMoveSound();
};

