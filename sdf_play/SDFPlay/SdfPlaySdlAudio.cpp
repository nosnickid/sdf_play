#include "SdfPlaySdlAudio.h"

#include "Debug.h"

const static int frequency = 22050;

static void mixaudio(void *unused, Uint8 *stream, int len) {
	SdfPlaySdlAudio *audio =  (SdfPlaySdlAudio *) unused;
	audio->tick(stream, len);
}

void SdfPlaySdlAudio::tick(Uint8 *stream, int len) {
	if (len != 2 * 512) fatal("Dodgy mixing bro: %d", len);

	// 512 16bit samples
	const int nosamples = 512;
	const int nobytes = nosamples * sizeof(Uint16);
	Uint8 data[nobytes];

	std::vector<AudioSource*>::iterator it;
	for(it = this->m_AudioSources.begin(); it != this->m_AudioSources.end(); it++) {
		if ((*it)->getAudio(data, nobytes, nosamples)) {
			SDL_MixAudio(stream, data, sizeof(data), SDL_MIX_MAXVOLUME);
		}
	}
}

void SdfPlaySdlAudio::registerAudioSource(AudioSource *src) {
	assERT(src != NULL, "Audio sources must be non-null");

	this->m_AudioSources.push_back(src);
	src->setParameters(frequency, sizeof(Uint16), false);
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
