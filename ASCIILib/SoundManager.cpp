#include "SoundManager.h"

#include <iostream>
using namespace std;

#include <cstdlib>
#include <time.h>

const int kChunkSize = 1024;

ascii::SoundManager::SoundManager(void)
    : mSoundVolume(1.0f)
{
    if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
    {
        cout << "Error! Failed to init with OGG support" << endl;
        cout << Mix_GetError() << endl;
    }

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

void ascii::SoundManager::loadSound(std::string key, const char* path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound)
    {
        cout << "Error! Failed to load sound " << path << endl;
        cout << Mix_GetError() << endl;
    }
	mSounds[key] = sound;
}

bool ascii::SoundManager::hasSound(std::string key)
{
    return mSounds.find(key) != mSounds.end();
}

void ascii::SoundManager::freeSound(std::string key)
{
	Mix_FreeChunk(mSounds[key]);
	mSounds.erase(key);
}

void ascii::SoundManager::playSound(std::string key, float volume)
{
    int channel = firstOpenChannel();

    Mix_Volume(channel, MIX_MAX_VOLUME * mSoundVolume * volume);
	Mix_PlayChannel(channel, mSounds[key], 0);
}

int ascii::SoundManager::soundDuration(std::string key)
{
    Mix_Chunk* chunk = mSounds[key];

    int ms = chunk->alen / ((44100*2)/1000);

    return ms;
}

int ascii::SoundManager::firstOpenChannel()
{
    // Return the first channel that's not playing a sample
    for (int i = 0; i < MIX_CHANNELS; ++i)
    {
        if (!Mix_Playing(i)) return i;
    }

    // There is no open channel
    return -1;
}
 
int ascii::SoundManager::soundDuration(Mix_Chunk* sound)
{
    int ms = sound->alen / ((44100*2)/1000);

    return ms;
}

int ascii::SoundManager::totalSoundDuration(std::vector<std::string> keys)
{
    int sum = 0;

    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        if (hasSound(*it))
        {
            sum += this->soundDuration(*it);
        }
    }

    return sum;
}

void ascii::SoundManager::loadGroupSound(std::string group, const char* path)
{
	mSoundGroups[group].push_back(Mix_LoadWAV(path));
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

int ascii::SoundManager::playSoundGroup(std::string group, float volume)
{
	ascii::SoundManager::SoundGroup soundGroup = mSoundGroups[group];

	int n = rand() % soundGroup.size();

    int channel = firstOpenChannel();
    Mix_Volume(channel, MIX_MAX_VOLUME * mSoundVolume * volume);
	return Mix_PlayChannel(channel, mSoundGroups[group][n], 0);
}

int ascii::SoundManager::playSoundGroupGetDuration(std::string group, float volume)
{
	ascii::SoundManager::SoundGroup soundGroup = mSoundGroups[group];

	int n = rand() % soundGroup.size();

    Mix_Chunk* groupSound = mSoundGroups[group][n];

    int channel = firstOpenChannel();
    Mix_Volume(channel, MIX_MAX_VOLUME * mSoundVolume * volume);
	Mix_PlayChannel(channel, groupSound, 0);

    return soundDuration(groupSound);
}

void ascii::SoundManager::loopSoundGroup(std::string group, float volume)
{
	ascii::SoundManager::SoundGroup soundGroup = mSoundGroups[group];

	int n = rand() % soundGroup.size();

    Mix_Chunk* groupSound = mSoundGroups[group][n];

    int channel = firstOpenChannel();
    Mix_Volume(channel, MIX_MAX_VOLUME * mSoundVolume * volume);
	Mix_PlayChannel(channel, groupSound, -1);
    mLoopingChannels[group] = channel;
}

void ascii::SoundManager::stopLoopingGroup(std::string group)
{
	Mix_HaltChannel(mLoopingChannels[group]);
	mLoopingChannels.erase(group);
}

void ascii::SoundManager::stopLoopingGroup()
{
	for (auto it = mLoopingChannels.begin(); it != mLoopingChannels.end(); ++it)
	{
		Mix_HaltChannel(it->second);
	}

	mLoopingChannels.clear();
}

void ascii::SoundManager::pauseSounds()
{
    Mix_Pause(-1);
}

void ascii::SoundManager::resumeSounds()
{
    Mix_Resume(-1);
}

float ascii::SoundManager::getSoundVolume()
{
	return mSoundVolume;
}

void ascii::SoundManager::setSoundVolume(float value)
{
    mSoundVolume = value;
}

void ascii::SoundManager::loadTrack(std::string key, const char* path)
{
    Mix_Music* track = Mix_LoadMUS(path);
    if (!track)
    {
        cout << "Failed to load music file " << path << " with error:" << endl;
        cout << Mix_GetError() << endl;
    }
	mTracks[key] = track;
}

void ascii::SoundManager::freeTrack(std::string key)
{
	Mix_FreeMusic(mTracks[key]);
	mTracks.erase(key);
}

void ascii::SoundManager::playTrack(std::string key, int loops)
{
    Mix_Music* track = mTracks[key];
	Mix_PlayMusic(track, loops);
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
