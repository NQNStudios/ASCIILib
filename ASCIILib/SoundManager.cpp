#include "SoundManager.h"

#include <iostream>
using namespace std;

#include <cstdlib>
#include <time.h>

#include "Log.h"
using ascii::Log;


const int kChunkSize = 1024;

ascii::SoundManager::SoundManager(void)
    : mSoundVolume(1.0f)
{
    if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
    {
        Log::Error("Failed to init with OGG support");
        Log::SDLError();
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

void ascii::SoundManager::update()
{
	for (auto it = mLoopingChannels.begin(); it != mLoopingChannels.end(); ++it)
	{
		if (!Mix_Playing(it->second.first))
		{
			mLoopingChannels[it->first] = std::make_pair(
                    playSoundGroup(it->first, it->second.second),
                    it->second.second);
		}
	}
}

void ascii::SoundManager::loadSound(std::string key, string path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (!sound)
    {
        Log::Error("Failed to load sound " + path);
        Log::SDLError();
    }
	mSounds[key] = sound;
}

bool ascii::SoundManager::hasSound(std::string key)
{
    return mSounds.find(key) != mSounds.end();
}

void ascii::SoundManager::freeSound(std::string key)
{
	Mix_FreeChunk(getSound(key));
	mSounds.erase(key);
}

void ascii::SoundManager::playSound(std::string key, float volume)
{
    int channel = firstOpenChannel();

    Mix_Volume(channel, MIX_MAX_VOLUME * (mSoundVolume * volume));
	Mix_PlayChannel(channel, getSound(key), 0);
}

void ascii::SoundManager::loopSound(std::string key, float volume)
{
    int channel = firstOpenChannel();

    // Store which channel it's looping on
    mLoopingSoundChannels[key] = channel;

    Mix_Volume(channel, MIX_MAX_VOLUME * (mSoundVolume * volume));
    Mix_PlayChannel(channel, getSound(key), -1);
}

void ascii::SoundManager::stopLoopingSound(std::string key)
{
    // Check which channel it's looping on
    int channel = mLoopingSoundChannels[key];

    Mix_HaltChannel(channel);

    // Forget which channel it was looping on
    mLoopingSoundChannels.erase(key);
}

int ascii::SoundManager::soundDuration(std::string key)
{
    Mix_Chunk* chunk = getSound(key);

    int ms = chunk->alen / ((44100*2)/1000);

    return ms;
}

int ascii::SoundManager::averageGroupSoundDuration(std::string groupKey)
{
    ascii::SoundManager::SoundGroup group = getSoundGroup(groupKey);

    int sum = 0;
    for (int i = 0; i < group.size(); ++i)
    {
        Mix_Chunk* sound = group[i];

        sum += this->soundDuration(sound);
    }

    return sum / group.size();
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

void ascii::SoundManager::loadGroupSound(std::string group, string path)
{
	mSoundGroups[group].push_back(Mix_LoadWAV(path.c_str()));
}

void ascii::SoundManager::freeSoundGroup(std::string group)
{
	ascii::SoundManager::SoundGroup soundGroup = getSoundGroup(group);

	for (auto it = soundGroup.begin(); it != soundGroup.end(); ++it)
	{
		Mix_FreeChunk(*it);
	}

	mSoundGroups.erase(group);
}

int ascii::SoundManager::playSoundGroup(std::string group, float volume)
{
	ascii::SoundManager::SoundGroup soundGroup = getSoundGroup(group);

    if (soundGroup.empty())
    {
        Log::Error("Tried to play empty sound group: " + group);
    }

	int n = rand() % soundGroup.size();

    int channel = firstOpenChannel();
    Mix_Volume(channel, MIX_MAX_VOLUME * (mSoundVolume * volume));
	return Mix_PlayChannel(channel, getSoundGroup(group)[n], 0);
}

int ascii::SoundManager::playSoundGroupGetDuration(std::string group, float volume)
{
	ascii::SoundManager::SoundGroup soundGroup = getSoundGroup(group);

	int n = rand() % soundGroup.size();

    Mix_Chunk* groupSound = getSoundGroup(group)[n];

    int channel = firstOpenChannel();
    Mix_Volume(channel, MIX_MAX_VOLUME * (mSoundVolume * volume));
	Mix_PlayChannel(channel, groupSound, 0);

    return soundDuration(groupSound);
}

void ascii::SoundManager::loopSoundGroup(std::string group, float volume)
{
    mLoopingChannels[group] = std::make_pair(playSoundGroup(group, volume), volume);
}

void ascii::SoundManager::stopLoopingGroup(std::string group)
{
	mLoopingChannels.erase(group);
}

void ascii::SoundManager::stopLoopingGroup()
{
	for (auto it = mLoopingChannels.begin(); it != mLoopingChannels.end(); ++it)
	{
		Mix_HaltChannel(it->second.first);
	}

	mLoopingChannels.clear();
}

void ascii::SoundManager::fadeOutLoopingGroup(std::string group, int ms)
{
    // Fade out the current sound
    int channel = mLoopingChannels[group].first;
    Mix_FadeOutChannel(channel, ms);

    // Stop looping
    stopLoopingGroup(group);
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

void ascii::SoundManager::loadTrack(std::string key, string path)
{
    Mix_Music* track = Mix_LoadMUS(path.c_str());
    if (!track)
    {
        Log::Error("Failed to load music file: " + path);
        Log::SDLError();
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
    mCurrentTrack = key;
}

void ascii::SoundManager::fadeInTrack(std::string key, int ms, int loops, double position)
{
	Mix_FadeInMusicPos(mTracks[key], loops, ms, position);
    mCurrentTrack = key;
}

void ascii::SoundManager::stopTrack()
{
	Mix_HaltMusic();
    mCurrentTrack = "";
}
			
void ascii::SoundManager::fadeOutTrack(int ms)
{
	Mix_FadeOutMusic(ms);
    mCurrentTrack = "";
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

Mix_Chunk* ascii::SoundManager::getSound(std::string key)
{
    if (!this->hasSound(key))
    {
        Log::Error("Tried to access nonexistent sound: " + key);
        return NULL;
    }

    Mix_Chunk* sound = mSounds[key];
    return sound;
}

ascii::SoundManager::SoundGroup ascii::SoundManager::getSoundGroup(
        std::string groupKey)
{
    if (mSoundGroups.find(groupKey) == mSoundGroups.end())
    {
        Log::Error("Tried to access nonexistent sound group: " + groupKey);
        return SoundGroup();
    }

    ascii::SoundManager::SoundGroup group = mSoundGroups[groupKey];
    return group;
}
