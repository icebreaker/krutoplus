#include "kruto/audio.hpp"
#include "kruto/version.hpp"
#include "kruto/console.hpp"
#include "kruto/file.hpp"

namespace kruto {

Audio::Audio(void) : mChunk(NULL), mChannel(-1), mLoop(false), mVolume(1.0f)
{
	// empty
}

Audio::~Audio(void)
{
	dispose();
}

const char *Audio::fileName(void) const
{
	return mFileName;
}

bool Audio::load(const char *pFileName)
{
	if(isLoaded())
		return true;

	if(File::isRelative(pFileName))
		return false;

	mChunk = Mix_LoadWAV(pFileName);
	if(mChunk == NULL)
		Console::log("[audio] Loading %s failed: %s", pFileName, Mix_GetError());
	else
		strncpy(mFileName, pFileName, 256);

	return isLoaded();
}

void Audio::dispose(void)
{
	if(mChannel != -1)
	{
		Mix_HaltChannel(mChannel);
		mChannel = -1;
	}

	if(mChunk != NULL)
	{
		Mix_FreeChunk(mChunk);
		mChunk = NULL;
	}
}

bool Audio::isLoaded(void) const
{
	return mChunk != NULL;	 
}

void Audio::play(void)
{
	if(!isLoaded())
		return;

	Mix_VolumeChunk(mChunk, mVolume * MIX_MAX_VOLUME);

	if(mChannel != -1 && Mix_Paused(mChannel))
	{
		Mix_Resume(mChannel);
	}
	else
	{
		if(mChannel != -1 && Mix_Playing(mChannel))
		{
			Mix_HaltChannel(mChannel);
			mChannel = -1;
		}
		mChannel = Mix_PlayChannel(-1, mChunk, mLoop ? -1 : 0);
	}	
}

void Audio::pause(void)
{
	if(mChannel != -1 && Mix_Playing(mChannel))
		Mix_Pause(mChannel);
}

void Audio::stop(void)
{
	if(mChannel != -1)
	{
		Mix_HaltChannel(mChannel);
		mChannel = -1;
	}	
}

bool Audio::isPlaying(void) const
{
	return mChannel != -1 && Mix_Playing(mChannel);
}

bool Audio::isPaused(void) const
{
	return mChannel != -1 && Mix_Paused(mChannel);
}

bool Audio::isStopped(void) const
{
	return mChannel == -1;	 
}

bool Audio::isLooping(void) const
{
	return mLoop;
}

void Audio::setLoop(const bool pLoop)
{
	mLoop = pLoop;
}

float Audio::volume(void) const
{
	return mVolume;
}

void Audio::setVolume(const float pVolume)
{
	mVolume = pVolume;

	if(mChunk != NULL)
		Mix_VolumeChunk(mChunk, pVolume * MIX_MAX_VOLUME);
}

bool Audio::initialize(void)
{
	Console::log("[audio] initialized");

#ifdef MIX_INIT_OGG
	if(Mix_Init(MIX_INIT_OGG) < 0)
	{
		Console::log("Failed to initialize SDL_mixer: %s", Mix_GetError());
		return false;
	}
#endif

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		Console::log("Failed to initialize SDL_mixer (Mix_OpenAudio): %s", Mix_GetError());
		return false;
	}

	return true;
}

void Audio::uninitialize(void)
{
	Mix_CloseAudio();

#ifdef MIX_INIT_OGG
	Mix_Quit();
#endif

	Console::log("[audio] uninitialized");
}

}
