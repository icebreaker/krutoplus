#ifndef KRAUDIO_HPP
#define KRAUDIO_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace kruto {

class Audio
{
public:
	Audio(void);
	~Audio(void);

	const char *fileName(void) const;

	bool load(const char *pFileName);
	void dispose(void);

	bool isLoaded(void) const;

	void play(void);
	void pause(void);
	void stop(void);

	bool isPlaying(void) const;
	bool isPaused(void) const;
	bool isStopped(void) const;
	bool isLooping(void) const;	

	void setLoop(const bool pLoop);

	float volume(void) const;
	void setVolume(const float pVolume);

public:
	static bool initialize(void);
	static void uninitialize(void);

private:
	Mix_Chunk *mChunk;
	int mChannel;
	bool mLoop;
	float mVolume;
	char mFileName[256];
};

}

#endif
