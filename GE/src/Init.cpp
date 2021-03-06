#include <Init.h>

/**
 * @brief
 * @param screen_width set window width
 * @param screen_height set window height
 */
GE::Init::Init(const int screen_width, const int screen_height)
{
	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Could not initialize! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//create window
		_window = SDL_CreateWindow("SDL Window",
								   SDL_WINDOWPOS_UNDEFINED,
								   SDL_WINDOWPOS_UNDEFINED,
								   screen_width,
								   screen_height,
								   SDL_WINDOW_SHOWN);
		if (_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//create renderer for window
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
			if (_renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			}
		}
	}
	TTF_Init();

	//initialize Audio
	if (SDL_Init(SDL_INIT_AUDIO) == -1)
	{
		printf("SDL Audio Could not initialize! SDL Error: %s\n", SDL_GetError());
	}

	int flags = MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_MP3;
	int initted = Mix_Init(flags);

	if (initted&flags != flags) {
		printf("Mix_Init: Failed to init required sound format!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
    	printf("Could not open Mix Audio! Mix_OpenAudio: %s\n", Mix_GetError());
	}
}

/**
 * @brief
 * class destructor
 */
GE::Init::~Init()
{
	//destroy window and renderer
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyTexture(_world);
	_window = NULL;
	_renderer = NULL;
	_world = NULL;
	//quit system
	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	delete(_box2d_world);
	_box2d_world = NULL;
}

/**
 * @brief
 * game loop
 */
bool GE::Init::update()
{
	SDL_Event e; //handle event
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
	}
	return true;
}

void GE::Init::render()
{
	//render color and clearing
	if (_world)
		SDL_SetRenderTarget(_renderer, _world);
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255); //set screen background color
	SDL_RenderClear(_renderer); //refresh renderer
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
}

/**
 * @brief
 * game update on loop
 */
void GE::Init::present()
{
	SDL_RenderPresent(_renderer); //update renderer
}

/**
 * @brief
 * get renderer of window
 */
SDL_Renderer* GE::Init::getRenderer()
{
	return _renderer;
}

SDL_Window* GE::Init::getWindow()
{
	return _window;
}

double GE::Init::getDeltaTime()
{
	//delta time
	_elapsedTime = SDL_GetTicks();
	double dt = (_elapsedTime - _lastFrameTimeElapsed) / 1000.0f;
	_lastFrameTimeElapsed = _elapsedTime;
	return dt;
}

void GE::Init::initCamera(int width, int height)
{
	_world = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_GetWindowSize(_window, &_camera.w, &_camera.h);
}

void GE::Init::setCamera(GE::Sprite* object)
{
	if (!_world) return;

	int world_width, world_height;
	SDL_QueryTexture(_world, NULL, NULL, &world_width, &world_height);
	
	_camera.x = object->getPositionX() - _camera.w / 4;

	if (_camera.x <= 0)
		_camera.x = 0;
	else if (_camera.x >= 640)
		_camera.x = 640;
}

void GE::Init::updateCamera()
{
	if (!_world) return;

	SDL_SetRenderTarget(_renderer, NULL);
	SDL_RenderCopy(_renderer, _world, &_camera, NULL);
}

int GE::Init::getCameraX()
{
	return _camera.x;
}

void GE::Init::removeCamera()
{
	SDL_DestroyTexture(_world);
	_world = NULL;
}

void GE::Init::initBox2DWorld(b2Vec2 gravity)
{
	_box2d_world = new b2World(gravity);
}

void GE::Init::updateBox2DWorld(double dt)
{
	float gravity = 400 * dt;
	if (gravity > 2) gravity = 2;
	_box2d_world->SetGravity(b2Vec2(0, gravity));
	_box2d_world->Step(_box2d_timestep, _velocity_iterations, _position_iterations);
}

b2World* GE::Init::getBox2DWorld()
{
	return _box2d_world;
}