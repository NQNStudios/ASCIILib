#pragma once

#include <map>
#include <string>
#include <vector>
#include <utility>

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
			
            void update();

			///<summary>
			/// Loads and stores a sound effect in the SoundManager.
			///</summary>
			///<param name="key">The key with which to store the sound.</param>
			///<param name="path">The file path of the WAV file.</param>
			void loadSound(std::string key, const char* path);

            ///<summary>
            /// Check if the SoundManager has loaded a sound corresponding
            /// to the given key
            ///</summary>
            bool hasSound(std::string key);

			///<summary>
			/// Frees a sound effect from memory.
			///</summary>
			///<param name="key">The key with which the sound is stored.</param>
			void freeSound(std::string key);

			///<summary>
			/// Plays a sound effect.
			///</summary>
			///<param name="key">The key with which the sound is stored.</param>
			void playSound(std::string key, float volume=1.0f);

            void loopSound(std::string key, float volume=1.0f);
            void stopLoopingSound(std::string key);

            ///<summary>
            /// Return the length in milliseconds of a sound effect recorded in
            /// Mono 44k 16-bit
            ///</summary>
            int soundDuration(std::string key);

            ///<summary>
            /// Return the summation of the lengths of every sound effect
            /// referenced by a key in the given collection
            ///</summary>
            int totalSoundDuration(std::vector<std::string> keys);
			
			///<summary>
			/// Loads and stores a sound effect in a sound group of the SoundManager.
			///</summary>
			///<param name="key">The group in which to store the sound.</param>
			///<param name="path">The file path of the WAV file.</param>
			void loadGroupSound(std::string group, const char* path);

			///<summary>
			/// Frees all sounds from a sound group.
			///</summary>
			void freeSoundGroup(std::string group);
			
			///<summary>
			/// Plays a random sound from the given sound group.
			///</summary>
			///<returns>The channel on which the sound group was played.</returns>
			int playSoundGroup(std::string group, float volume=1.0f);

			///<summary>
			/// Play a random sound from the given sound group and return its
            /// sound duration
			///</summary>
			///<returns>The duration of the sound which was played.</returns>
            int playSoundGroupGetDuration(std::string group, float volume=1.0f);

			///<summary>
			/// Starts looping a sound group, randomly selecting sounds from it to play one after the other.
			///</summary>
			void loopSoundGroup(std::string group, float volume=1.0f);

			///<summary>
			/// Stops looping a sound group.
			///</summary>
			void stopLoopingGroup(std::string group);

			///<summary>
			/// Stops looping all current looping sound groups.
			///</summary>
			void stopLoopingGroup();

            ///<summary>
            /// Pauses all sound effects currently playing
            ///</summary>
            void pauseSounds();

            ///<summary>
            /// Resumes all sound effects currently paused
            ///</summary>
            void resumeSounds();


			///<summary>The current sound volume, from 0 to 1.</summary>
			float getSoundVolume();

			///<summary>Sets the current sound volume, from 0 to 1.</summary>
			void setSoundVolume(float value);



			///<summary>
			/// Loads and stores a music track in the SoundManager.
			///</summary>
			///<param name="key">The key with which to store the track.</param>
			///<param name="path">The file path of the track.</param>
			void loadTrack(std::string key, const char* path);

			///<summary>
			/// Frees a music track from memory.
			///</summary>
			///<param name="key">The key with which the track is stored.</param>
			void freeTrack(std::string key);

			///<summary>
			/// Begins playing a music track from the SoundManager.
			///</summary>
			///<param name="key">The key with which the track is stored.</param>
			///<param name="loops">The number of times to loop the track. 
			///If -1, the track will loop infinitely. If 0, the track will play once.</param>
			void playTrack(std::string key, int loops = -1);

			///<summary>
			/// Fades in a music track from the SoundManager.
			///</summary>
			///<param name="key">The key with which the track is stored.</param>
			///<param name="ms">The number of milliseconds the fade effect will take.</param>
			///<param name="loops">The number of times to loop the track. 
			///If -1, the track will loop infinitely. If 0, the track will play once.</param>
			///<param name="position">The position at which to start the track.</param>
			void fadeInTrack(std::string key, int ms, int loops = -1, double position = 0.0);
			
			///<summary>Stops the current music track.</summary>
			void stopTrack();

			///<summary>
			/// Fades out the current music track.
			///</summary>
			///<param name="ms">The number of milliseconds the fade effect will take.</param>
			void fadeOutTrack(int ms);

			///<summary>Pauses the current music track.</summary>
			void pauseTrack();

			///<summary>Resumes the current music track.</summary>
			void resumeTrack();

			///<summary>Rewinds the current music track to the beginning.</summary>
			void rewindTrack();

			///<summary>Sets the position of the track currently playing.</summary>
			void setTrackPosition(double position);

			///<summary>The current music volume, from 0 to 1.</summary>
			float getMusicVolume();

			///<summary>Sets the current music volume, from 0 to 1.</summary>
			void setMusicVolume(float volume);

            std::string currentTrackName() { return mCurrentTrack; }

			///<summary>Whether a music track is currently playing.</summary>
			bool trackPlaying() { return Mix_PlayingMusic() != 0; }

			///<summary>Whether a music track is currently paused.</summary>
			bool trackPaused() { return Mix_PausedMusic() != 0; }

			///<summary>The status of the current music fade effect.</summary>
			Mix_Fading fadingMusic() { return Mix_FadingMusic(); }
		private:
            ///<summary>
            /// Return the length in milliseconds of a sound effect recorded in
            /// Mono 44k 16-bit
            ///</summary>
            int soundDuration(Mix_Chunk* sound);

            ///<summary>
            /// Return the number of the first channel that's free to play
            /// audio
            ///</summary>
            int firstOpenChannel();

            Mix_Chunk* getSound(std::string key);

			typedef std::vector<Mix_Chunk*> SoundGroup;

			std::map<std::string, Mix_Chunk*> mSounds;
			std::map<std::string, SoundGroup> mSoundGroups;

			std::map<std::string, Mix_Music*> mTracks;

			std::map<std::string, std::pair<int, float> > mLoopingChannels;
            std::map<std::string, int> mLoopingSoundChannels;

            std::string mCurrentTrack;

            float mSoundVolume;
	};

};
