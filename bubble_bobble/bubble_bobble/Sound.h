#pragma once
#ifndef _CSOUND_H_
#define _CSOUND_H_

#include <fmod.h>

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f


class Sound {
private:
	static FMOD_SYSTEM *soundSystem;

	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel;

	float volume;
	FMOD_BOOL fmodBool;

public:
	Sound(const char* path, bool loop);
	~Sound();

	static int Init();
	static int Release();

	float GetVolume();
	void SetVolume(float vol);

	int play(float vol);
	int pause();
	int resume();
	int stop();
	int volumeUp();
	int volumeDown();

	int Update();
};

#endif