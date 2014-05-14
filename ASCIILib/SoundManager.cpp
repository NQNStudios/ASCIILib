#include "SoundManager.h"

const int kChunkSize = 1024;

ascii::SoundManager::SoundManager(void)
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, kChunkSize);
}

ascii::SoundManager::~SoundManager(void)
{
	for (auto it = mSounds.begin(); it != mSounds.end(); ++it)
	{
		Mix_FreeChunk(it->second);
	}

	for (auto it = mTracks.begin(); it != mTracks.end(); ++it)
	{
		Mix_FreeMusic(it->second);
	}

	Mix_CloseAudio();
}

void ascii::SoundManager::addSound(const char* key, const char* path)
{
	mSounds[key] = Mix_LoadWAV(path);
}

void ascii::SoundManager::playSound(const char* key)
{
	Mix_PlayChannel(-1, mSounds[key], 0);
}

float ascii::SoundManager::getSoundVolume()
{
	return (float) Mix_Volume(-1, -1) / MIX_MAX_VOLUME;
}

void ascii::SoundManager::setSoundVolume(float value)
{
	Mix_Volume(-1, MIX_MAX_VOLUME * value);
}

void ascii::SoundManager::addTrack(const char* key, const char* path)
{
	mTracks[key] = Mix_LoadMUS(path);
}

void ascii::SoundManager::playTrack(const char* key, int loops)
{
	Mix_PlayMusic(mTracks[key], loops);
}

void ascii::SoundManager::fadeInTrack(const char* key, int loops, int ms, double position)
{
	Mix_FadeInMusicPos(mTracks[key], loops, ms, position);
}

void ascii::SoundManager::stopTrack()
{
	Mix_HaltMusic();
}
			
void ascii::SoundManager::fadeOutTrack(int ms)
{
	Mix_FadeOutMusic(ms);
}

void ascii::SoundManager::pauseTrack()
{
	Mix_PauseMusic();
}

void ascii::SoundManager::resumeTrack()
{
	Mix_ResumeMusic();
}

void ascii::SoundManager::rewindTrack()
{
	Mix_RewindMusic();
}

void ascii::SoundManager::setTrackPosition(double position)
{
	Mix_SetMusicPosition(position);
}

float ascii::SoundManager::getMusicVolume()
{
	return (float) Mix_VolumeMusic(-1) / MIX_MAX_VOLUME;
}

void ascii::SoundManager::setMusicVolume(float value)
{
	Mix_VolumeMusic(MIX_MAX_VOLUME * value);
}