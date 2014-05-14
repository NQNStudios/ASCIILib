#pragma once

#include <map>

#include <SDL_mixer.h>

namespace ascii
{

	///<summary>
	/// Loads, stores and plays all of the game's sound effects and music.
	///</summary>
	class SoundManager
	{
		public:
			SoundManager();
			~SoundManager();
			
			void addSound(const char* key, const char* path);
			void playSound(const char* key);

			float getSoundVolume();
			void setSoundVolume(float value);

			void addTrack(const char* key, const char* path);
			void playTrack(const char* key, int loops = -1);
			void fadeInTrack(const char* key, int loops, int ms, double position = 0.0);
			
			void stopTrack();
			void fadeOutTrack(int ms);

			void pauseTrack();
			void resumeTrack();
			void rewindTrack();
			void setTrackPosition(double position);

			float getMusicVolume();
			void setMusicVolume(float volume);

			bool trackPlaying() { return Mix_PlayingMusic(); }
			bool trackPaused() { return Mix_PausedMusic(); }
			Mix_Fading fadingMusic() { return Mix_FadingMusic(); }
		private:
			std::map<const char*, Mix_Chunk*> mSounds;
			std::map<const char*, Mix_Music*> mTracks;
	};

};