#pragma once
#include "fmod.hpp"
#include <iostream>

class SoundManager {
public:
	void start() {
		init();
		playbackSound();
	}

	void end() {
		cleanup();
	}
private:
	FMOD::System *system = nullptr;
	FMOD::ChannelGroup *channelGroup;
	FMOD::Channel* channel;
	FMOD::Sound* sound;

	void init() {
		if (FMOD::System_Create(&system) != FMOD_OK)
			throw std::runtime_error("Error creating sound system");

		if (system->init(512, FMOD_INIT_NORMAL, 0) != FMOD_OK)
			throw std::runtime_error("Error initialising sound system");
	}

	void playbackSound() {
		system->createChannelGroup("ch1", &channelGroup);

		system->createSound("time_filter.wav", FMOD_DEFAULT, nullptr, &sound);

		system->playSound(sound, channelGroup, false, &channel);
		channel->setVolume(0.2f);
	}

	void cleanup() {
		sound->release();
		channel->stop();
		channelGroup->stop();
		system->close();
	}
};