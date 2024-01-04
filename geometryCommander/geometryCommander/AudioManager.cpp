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
	if (!attackSound.loadFromFile("sounds/attack.wav")) {
		std::cout << "error loading atk sound";
	}
	if (!hitSound.loadFromFile("sounds/hit.wav")) {
		std::cout << "error loading hit sound";
	}
	if (!grenadeSound.loadFromFile("sounds/grenade.wav")) {
		std::cout << "error loading grenade sound";
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

void AudioManager::PlayHitSound()
{
	if (playback.getStatus() != sf::SoundSource::Status::Playing) {
		playback.setBuffer(hitSound);
		playback.play();
	}
}

void AudioManager::PlayAttackSound()
{
	if (playback.getStatus() != sf::SoundSource::Status::Playing) {
		playback.setBuffer(attackSound);
		playback.play();
	}
}

void AudioManager::PlayGrenadeSound()
{
	if (playback.getStatus() != sf::SoundSource::Status::Playing) {
		playback.setBuffer(grenadeSound);
		playback.play();
	}
}
