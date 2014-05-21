#include "SoundManager.h"

#include <cstdlib>
#include <time.h>
#include <iostream>

const int kChunkSize = 1024;

ascii::SoundManager::SoundManager(void)
	: mLoopingGroup(""), mLoopingChannel(-1)
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, kChunkSize);

	srand(time(NULL));
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

void ascii::SoundManager::update()
{
	if (mLoopingGroup.compare(""))
	{
		if (!Mix_Playing(mLoopingChannel))
		{
			mLoopingChannel = playSoundGroup(mLoopingGroup);
		}
	}
}

void ascii::SoundManager::loadSound(std::string key, const char* path)
{
	mSounds[key] = Mix_LoadWAV(path);

	std::cout << "Sound load error: " << Mix_GetError() << std::endl;
}

void ascii::SoundManager::freeSound(std::string key)
{
	Mix_FreeChunk(mSounds[key]);
	mSounds.erase(key);
}

void ascii::SoundManager::playSound(std::string key)
{
	Mix_PlayChannel(-1, mSounds[key], 0);
}

void ascii::SoundManager::loadGroupSound(std::string group, const char* path)
{
	mSoundGroups[group].push_back(Mix_LoadWAV(path));

	std::cout << "Group sound load error: " << Mix_GetError() << std::endl;
}

void ascii::SoundManager::freeSoundGroup(std::string group)
{
	ascii::SoundManager::SoundGroup soundGroup = mSoundGroups[group];

	for (auto it = soundGroup.begin(); it != soundGroup.end(); ++it)
	{
		Mix_FreeChunk(*it);
	}

	mSoundGroups.erase(group);
}

int ascii::SoundManager::playSoundGroup(std::string group)
{
	ascii::SoundManager::SoundGroup soundGroup = mSoundGroups[group];

	int n = rand() % soundGroup.size();

	return Mix_PlayChannel(-1, mSoundGroups[group][n], 0);
}

void ascii::SoundManager::loopSoundGroup(std::string group)
{
	mLoopingGroup = group;
	mLoopingChannel = playSoundGroup(group);
}

void ascii::SoundManager::stopLoopingGroup()
{
	Mix_HaltChannel(mLoopingChannel);

	mLoopingGroup = "";
	mLoopingChannel = -1;
}

float ascii::SoundManager::getSoundVolume()
{
	return (float) Mix_Volume(-1, -1) / MIX_MAX_VOLUME;
}

void ascii::SoundManager::setSoundVolume(float value)
{
	Mix_Volume(-1, MIX_MAX_VOLUME * value);
}

void ascii::SoundManager::loadTrack(std::string key, const char* path)
{
	mTracks[key] = Mix_LoadMUS(path);
}

void ascii::SoundManager::freeTrack(std::string key)
{
	Mix_FreeMusic(mTracks[key]);
	mTracks.erase(key);
}

void ascii::SoundManager::playTrack(std::string key, int loops)
{
	Mix_PlayMusic(mTracks[key], loops);
}

void ascii::SoundManager::fadeInTrack(std::string key, int ms, int loops, double position)
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