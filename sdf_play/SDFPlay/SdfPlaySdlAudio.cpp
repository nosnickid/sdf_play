#include "SdfPlaySdlAudio.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "Debug.h"

static int offs = 0;

const static int frequency = 22050;

static void mixaudio(void *unused, Uint8 *stream, int len) {
	if (len != 2 * 512) fatal("Dodgy mixing bro: %d", len);

	// 512 16bit samples
	const int nosamples = 512;
	const int nobytes = nosamples * sizeof(Uint16);
	Uint8 data[nobytes];
	Sint16 *ptr = (Sint16*)data;

	float timePerSample = 1.0f / (float)frequency;
	float timeInSeconds = timePerSample * offs;
	for(int i = 0; i < nosamples; i++, timeInSeconds += timePerSample) {
		ptr[i] = 16384 * sin(2600 * timeInSeconds * 2 * M_PI);
	}

	SDL_MixAudio(stream, data, sizeof(data), SDL_MIX_MAXVOLUME);

	offs += nosamples;
}

void SdfPlaySdlAudio::init(void) {
	SDL_AudioSpec fmt;

	/* Set 16-bit mono audio at 22Khz */
	fmt.freq = frequency;
	fmt.format = AUDIO_S16;
	fmt.channels = 1;
	fmt.samples = 512; /* A good value for games */
	fmt.callback = mixaudio;
	fmt.userdata = (void*)this;

	/* Open the audio device and start playing sound! */
	if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
		warning("Unable to open audio: %s\n", SDL_GetError());
	} else {
		info("Opened audio");
		SDL_PauseAudio(0);
	}
}
