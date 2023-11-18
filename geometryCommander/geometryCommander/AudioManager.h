#pragma once
#define SFML_STATIC
#include <SFML/Audio.hpp>
class AudioManager
{
private:
	sf::Sound playback;
	sf::SoundBuffer moveSound;
	sf::SoundBuffer invalidInputSound;
	sf::SoundBuffer dieSound;
	sf::SoundBuffer attackSound;
	sf::SoundBuffer hitSound;
	AudioManager();
	static AudioManager* instance;
public:
	static AudioManager* GetInstance();
	~AudioManager();
	void PlayMoveSound();
	void PlayInvalidInputSound();
	void PlayDieSound();
	void PlayHitSound();
	void PlayAttackSound();
};

