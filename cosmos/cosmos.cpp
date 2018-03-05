/*
*		COSMOS Alpha For Linux And Windows
*			
*			GCC | MSVC
*/

#include "cosmos.h"
#include "cpu.h"

uint8_t keymap[16] = {
	SDLK_x, SDLK_1,	SDLK_2,	SDLK_3,	//Row-1 [1,2,3,4]
	SDLK_q,	SDLK_w,	SDLK_e,	SDLK_a,	//Row-2 [q,w,e,r]
	SDLK_s,	SDLK_d,	SDLK_z,	SDLK_c,	//Row-3 [a,s,d,f]
	SDLK_4,	SDLK_r,	SDLK_f,	SDLK_v,	//Row-4 [z,x,c,v]
};

RCA1802 cpu = RCA1802();	// Virtual MachineObjectVirtual Machine Obj.
uint32_t ticks;				// VM execution time, (milliseconds)
uint32_t sdtick = 0;

void toggleDebug() {

	if (cpu.debug) {
		hideConsole(0);
		
		cpu.debug = false;
	}
	else {
		hideConsole(9);
		
		cpu.debug = true;
	}
}

void initSDL() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cerr << "[ERROR] [SDL] Failed to Initialize\n";
		exit(1);
	}

	window = SDL_CreateWindow("COSMOS-AlphaBuild", POS_UNDEF, POS_UNDEF, WIDTH*MULT, HEIGHT*MULT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

	audData = (AudioData*)malloc(sizeof(struct AudioData));
	audData->pos = 0;
	audData->delta = (float)6.283185307 * 1000 / 96000;

	audSpec = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

	audSpec->freq = 96000;
	audSpec->format = AUDIO_U8;
	audSpec->channels = 1;
	audSpec->samples = 1024;
	audSpec->callback = audCallback;
	audSpec->userdata = audData;

	audDevID = SDL_OpenAudioDevice(NULL, 0, audSpec, NULL, SDL_AUDIO_ALLOW_FORMAT_CHANGE);



	if (window == NULL) { cerr << "[ERROR][SDL] Failed to Create Window\n"; exit(2); }

}

void quitSDL() {
	free(audData);
	SDL_CloseAudioDevice(audDevID);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char *argv[]) {

	cpu.debug = false;
	initConsole();

	//if (argc != 2) { cerr << "Usage: cpu <ROM file>" << endl;	system("pause");	return 1;	}

	initSDL();	//	Init SDL Audio, Video and Input.

	cpu.resetState();
	if (!cpu.loadBinary(argv[1])) { return 2; }	//Load ROM
	hideConsole(9);
	// Main Loop (60Hz)
	while (true) {

		cpu.fetch(); cpu.execute();


		if (cpu.dT > 0) { --cpu.dT; } // Delay Timer

		// Sound Timer implementation.
		if ((int)cpu.sT == 0) {
			SDL_PauseAudioDevice(audDevID, 1);
		}
		else {
			SDL_PauseAudioDevice(audDevID, 0);
			--cpu.sT;
		}

		ticks = SDL_GetTicks();
		// Process SDL events
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) { exit(0); }

			// *** KEYDOWN EVENTS ***
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) { quitSDL(); exit(0); }

				if (e.key.keysym.sym == SDLK_F1) {
					cpu.resetState();
					if (!cpu.loadBinary(argv[1])) { return 2; }
				}

				if (e.key.keysym.sym == SDLK_F3) {
					MULT += 2;
					quitSDL();
					initSDL();
				}
				if (e.key.keysym.sym == SDLK_F4) {
					if (MULT != 2) {
						MULT -= 2;
						quitSDL();
						initSDL();
					}
				}

				if (e.key.keysym.sym == SDLK_SPACE) {
					limitFPS = false;
				}

				if (e.key.keysym.sym == SDLK_KP_DIVIDE) { toggleDebug(); }

				for (int i = 0; i < 16; ++i) {
					if (e.key.keysym.sym == keymap[i]) { cpu.keys[i] = 1; }
				}
			}
			
			// *** KEYUP EVENTS ***

			if (e.type == SDL_KEYUP) {

				if (e.key.keysym.sym == SDLK_SPACE) {
					limitFPS = true;
				}
				for (int i = 0; i < 16; ++i) {
					if (e.key.keysym.sym == keymap[i]) { cpu.keys[i] = 0; }
				}
			}

		}

		// If draw occurred, redraw SDL screen
		if (cpu.drawFlag) {
			cpu.drawFlag = false;

			// Update SDL texture (Slow Function)
			SDL_UpdateTexture(texture, NULL, cpu.screen, 256);
			// Clear screen and render
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		// SDL Slowdown: LoopTime_Spent + X = 16.6 millisecs

		sdtick++;
		if (sdtick >= 7) { sdtick = 0; } //60HZ

		if ((1000 / 500 > SDL_GetTicks() - ticks) && limitFPS) { //500HZ
			SDL_Delay(1000 / 500 - (SDL_GetTicks() - ticks));
		}
	}
}
