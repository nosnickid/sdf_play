#ifndef SDF_PLAY_DTMF_AUDIO_SOURCE_H__
#define SDF_PLAY_DTMF_AUDIO_SOURCE_H__

#include "AudioSource.h"

class DtmfAudioSource: public AudioSource {
private:
	int m_Offs;
	int m_CurrentTone;

public:
	static const int FREQ_697 = (1u << 0);
	static const int FREQ_770 = (1u << 1);
	static const int FREQ_852 = (1u << 2);
	static const int FREQ_941 = (1u << 3);
	static const int FREQ_1209 = (1u << 4);
	static const int FREQ_1336 = (1u << 5);
	static const int FREQ_1477 = (1u << 6);
	static const int FREQ_1633 = (1u << 7);
	
	static const int KEY_1 = DtmfAudioSource::FREQ_697 | DtmfAudioSource::FREQ_1209;
	static const int KEY_2 = DtmfAudioSource::FREQ_697 | DtmfAudioSource::FREQ_1336;
	static const int KEY_3 = DtmfAudioSource::FREQ_697 | DtmfAudioSource::FREQ_1477;
	static const int KEY_A = DtmfAudioSource::FREQ_697 | DtmfAudioSource::FREQ_1633;

	static const int KEY_4 = DtmfAudioSource::FREQ_770 | DtmfAudioSource::FREQ_1209;
	static const int KEY_5 = DtmfAudioSource::FREQ_770 | DtmfAudioSource::FREQ_1336;
	static const int KEY_6 = DtmfAudioSource::FREQ_770 | DtmfAudioSource::FREQ_1477;
	static const int KEY_B = DtmfAudioSource::FREQ_770 | DtmfAudioSource::FREQ_1633;

	static const int KEY_7 = DtmfAudioSource::FREQ_852 | DtmfAudioSource::FREQ_1209;
	static const int KEY_8 = DtmfAudioSource::FREQ_852 | DtmfAudioSource::FREQ_1336;
	static const int KEY_9 = DtmfAudioSource::FREQ_852 | DtmfAudioSource::FREQ_1477;
	static const int KEY_C = DtmfAudioSource::FREQ_852 | DtmfAudioSource::FREQ_1633;

	static const int KEY_ASTERISK = DtmfAudioSource::FREQ_941 | DtmfAudioSource::FREQ_1209;
	static const int KEY_0 = DtmfAudioSource::FREQ_941 | DtmfAudioSource::FREQ_1336;
	static const int KEY_HASH = DtmfAudioSource::FREQ_941 | DtmfAudioSource::FREQ_1477;
	static const int KEY_D = DtmfAudioSource::FREQ_941 | DtmfAudioSource::FREQ_1633;


	DtmfAudioSource();
	virtual bool getAudio(Uint8* dest, int noBytes, int noSamples);

	void playTone(int tone);
};

#endif