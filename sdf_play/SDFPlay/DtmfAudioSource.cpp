#include "DtmfAudioSource.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

DtmfAudioSource::DtmfAudioSource():
	m_Offs(0), m_CurrentTone(0) {
}

bool DtmfAudioSource::getAudio(Uint8* dest, int len, int noSamples) {
	if (!this->m_CurrentTone) return false;

	float timePerSample = 1.0f / (float)this->outputFreq;
	float timeInSeconds = timePerSample * this->m_Offs;

	float freq[2] = { 0, 0 };

	if (this->m_CurrentTone & FREQ_697) {
		freq[0] = 697;
	} else if (this->m_CurrentTone & FREQ_770) {
		freq[0] = 770;
	} else if (this->m_CurrentTone & FREQ_852) {
		freq[0] = 852;
	} else if (this->m_CurrentTone & FREQ_941) {
		freq[0] = 941;
	}

	if (this->m_CurrentTone & FREQ_1209) {
		freq[1] = 1209;
	} else if (this->m_CurrentTone & FREQ_1336) {
		freq[1] = 1336;
	} else if (this->m_CurrentTone & FREQ_1477) {
		freq[1] = 1477;
	} else if (this->m_CurrentTone & FREQ_1633) {
		freq[1] = 1633;
	}

	freq[0] *= 2 * M_PI;
	freq[1] *= 2 * M_PI;


	Uint16* ptr = (Uint16*)dest;

	for(int i = 0; i < noSamples; i++, timeInSeconds += timePerSample) {
		float dtmf[2];
		dtmf[0] = sin(freq[0] * timeInSeconds);
		dtmf[1] = sin(freq[1] * timeInSeconds);
		ptr[i] = std::min<float>(16384, std::max<float>(-16384, 16384 * (0.5 * dtmf[0] + 0.5*dtmf[1])  ));
	}

	this->m_Offs += noSamples;
	if (m_Offs > (this->outputFreq * 5)) m_Offs -= (this->outputFreq * 5);

	return true;
}

void DtmfAudioSource::playTone(int tone) {
	this->m_CurrentTone = tone;
}