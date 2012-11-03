#ifndef SDF_PLAY_AUDIO_SOURCE_H__
#define SDF_PLAY_AUDIO_SOURCE_H__

#include <SDL.h>

class AudioSource {
protected:
	int outputFreq;
	int outputSampleSize;
	bool outputStereo;
	
public:
	virtual void setParameters(int outputFreq, int outputSampleSize, bool outputStereo) {
		this->outputFreq = outputFreq;
		this->outputSampleSize = outputSampleSize;
		this->outputStereo = outputStereo;
	}

	virtual ~AudioSource() {
	};
	/**
	 * \brief Get the next batch of audio to mix in from the source.
	 * \param dest      destination for audio samples, provided by the audio engine.
	 * \param noBytes   number of bytes pointed to by dest.
	 * \param noSAmples number of samples that can be fit into this memory
	 * @
	 */
	virtual bool getAudio(Uint8 *dest, int noBytes, int noSamples) = 0;
};


#endif