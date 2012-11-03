#ifndef SDF_PLAY_FPS_COUNTER__
#define SDF_PLAY_FPS_COUNTER__

#include <SDL.h>
#include "RWText.h"

class FpsCounter {
	int m_CurrSecondMS;
	int m_Fps[5];
	int m_ptr;
	int m_frames;

public:
	FpsCounter(): 
		m_ptr(0), m_frames(0) {
		memset(m_Fps, 0, sizeof(m_Fps));
	}
public:
	void tickFrame() {
		int ticks = SDL_GetTicks();

		if (ticks / 1000 != m_CurrSecondMS) {
			m_Fps[m_ptr % 5] = m_frames;

			m_CurrSecondMS = ticks / 1000;
			m_frames = 0;
			m_ptr++;
		}

		m_frames++;

		int fps = (m_Fps[0] + m_Fps[1] + m_Fps[2] + m_Fps[3] + m_Fps[4]) / 5;
		rwtext.print(0, 0, "FPS: [%d]", fps);
	}
};


#endif