#ifndef INIT_H
#define INIT_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Sprite.h"

namespace GE
{
	class Init
	{
	private:
		SDL_Window *_window = NULL; //game window
		SDL_Renderer *_renderer = NULL; //game renderer
		SDL_Texture *_world = NULL;
		SDL_Rect _camera = {0, 0, 0, 0};
		Uint32 _elapsedTime = 0;
		Uint32 _lastFrameTimeElapsed = 0;
		Uint64 _start = 0;
		Uint64 _end = 0;

	public:
		Init(const int screen_width, const int screen_height);
		~Init();
		bool update();
		void present();
		SDL_Renderer* getRenderer();
		SDL_Window* getWindow();
		double getDeltaTime();
		void initCamera(int width, int height);
		void setCamera(GE::Sprite* object);
		void updateCamera();
	};
} // namespace GE

#endif