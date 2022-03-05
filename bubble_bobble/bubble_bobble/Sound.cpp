#include "Sound.h"
#include "framework.h"

FMOD_SYSTEM* Sound::soundSystem;

Sound::Sound(const char* path, bool loop)
{
	if (loop) FMOD_System_CreateSound(soundSystem, path, FMOD_LOOP_NORMAL, 0, &sound);
	else FMOD_System_CreateSound(soundSystem, path, FMOD_DEFAULT, 0, &sound);

	channel = nullptr;
	volume = SOUND_DEFAULT;
}

Sound::~Sound()
{
	FMOD_Sound_Release(sound);
}


int Sound::Init()
{
	FMOD_System_Create(&soundSystem);
	FMOD_System_Init(soundSystem, 32, FMOD_INIT_NORMAL, NULL);

	return 0;
}

int Sound::Release()
{
	FMOD_System_Close(soundSystem);
	FMOD_System_Release(soundSystem);

	return 0;
}

float Sound::GetVolume()
{
	return volume;
}

void Sound::SetVolume(float vol)
{
	volume = vol;
	FMOD_Channel_SetVolume(channel, volume);
}


int Sound::play(float vol)
{
	FMOD_System_PlaySound(soundSystem, sound, NULL, false, &channel);
	
	volume = vol;
	FMOD_Channel_SetVolume(channel, volume);
	
	return 0;
}

int Sound::pause()
{
	FMOD_Channel_SetPaused(channel, true);

	return 0;
}

int Sound::resume()
{
	FMOD_Channel_SetPaused(channel, false);

	return 0;
}

int Sound::stop()
{
	FMOD_Channel_Stop(channel);

	return 0;
}

int Sound::volumeUp()
{
	if (volume < SOUND_MAX) volume += SOUND_WEIGHT;

	FMOD_Channel_SetVolume(channel, volume);

	return 0;
}

int Sound::volumeDown()
{
	if (volume > SOUND_MIN) volume -= SOUND_WEIGHT;

	FMOD_Channel_SetVolume(channel, volume);

	return 0;
}


int Sound::Update()
{
	FMOD_Channel_IsPlaying(channel, &fmodBool);

	if (fmodBool) FMOD_System_Update(soundSystem);

	return 0;
}
