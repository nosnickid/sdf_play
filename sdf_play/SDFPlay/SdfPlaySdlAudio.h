#ifndef SDF_PLAY_SDL_AUDIO_H__
#define SDF_PLAY_SDL_AUDIO_H__

#include <SDL.h>
#include <SDL_audio.h>
#include <vector>

#include "AudioSource.h"

class SdfPlaySdlAudio {
private:
	std::vector<AudioSource*> m_AudioSources;

public:
	void init(void);

	void tick(Uint8 *stream, int len);
	void registerAudioSource(AudioSource *src);
};

#endif