#ifndef COSMOS_H
#define COSMOS_H

#include <iostream>
#include <stdint.h>
#include <stdlib.h>

#ifdef _WIN64
#include "cosmos-win.h"
#endif // _WIN64

#ifdef __linux__
#include "cosmos-lin.h"
#endif // linux

#define POS_UNDEF	SDL_WINDOWPOS_UNDEFINED_DISPLAY(0)

using namespace std;


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_AudioSpec* audSpec;
SDL_AudioDeviceID audDevID;
struct AudioData* audData;

uint32_t TARGET_FPS = 60;
uint8_t TARGET_IPS = 10;
uint8_t MULT = 16;
bool limitFPS = true;

struct AudioData { double pos; double delta; };

static void audCallback(void* userdata, Uint8* stream, int len) {
	struct AudioData* audData = (struct AudioData*) userdata;
	for (int i = 0; i < len; i++) {
		stream[i] = sin(audData->pos) + 0x7F;
		audData->pos += audData->delta;
		if (audData->pos > 1000) { audData->pos = 0; }
	}
}

#endif // COSMOS_H
