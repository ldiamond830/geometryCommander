#pragma once
#include <SFML/Audio.hpp>
class AudioManager
{
private:
	AudioManager();
	sf::Sound output;
	sf::SoundBuffer MoveSound;
	sf::SoundBuffer HitSound;
	sf::SoundBuffer MissSound;
	sf::SoundBuffer DeathSound;
	static AudioManager* instance;
	~AudioManager();
public:
	static AudioManager* GetInstance();
	void PlayMoveSound();
};

