#include <Sprite.h>

/**
 * @brief
 * @param init passing GE::Init object to get current renderer
 */
GE::Sprite::Sprite(SDL_Renderer* renderer, std::string image = "")
{
	_renderer = renderer;
	if (image != "")
		loadImage(image);
}

/**
 * @brief
 * class destructor
 */
GE::Sprite::~Sprite()
{
	//free texture
	SDL_DestroyTexture(_texture);
	_texture = NULL;
	_renderer = NULL;
	_animation.clear();
}

//load sprite from image
/**
 * @brief
 * @param imagePath load image from imagePath
 */
void GE::Sprite::loadImage(std::string imagePath)
{
	//load image from file
	SDL_Surface *image = IMG_Load(imagePath.c_str());
	_filename = imagePath;
	if (image == NULL)
	{
		printf("Unable to load image! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//create texture
		SDL_DestroyTexture(_texture);
		_texture = SDL_CreateTextureFromSurface(_renderer, image);
		if (_texture == NULL)
		{
			printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());
		}
	}
	_rect.w = image->w;
	_rect.h = image->h;
	//free surface
	SDL_FreeSurface(image);
}

SDL_Rect GE::Sprite::getRect()
{
	SDL_Rect rect = {(int)_position_x, (int)_position_y, _rect.w, _rect.h};
	return rect;
}

double GE::Sprite::getPositionX()
{
	return _position_x;
}

double GE::Sprite::getPositionY()
{
	return _position_y;
}

void GE::Sprite::setClip(SDL_Rect r)
{
	_rect = r;
}

void GE::Sprite::setClip(double r[4])
{
	_rect.x = r[0];
	_rect.y = r[1];
	_rect.w = r[2];
	_rect.h = r[3];
}

void GE::Sprite::setAlpha(double alpha)
{
	_alpha = alpha;
	SDL_SetTextureAlphaMod(_texture, _alpha);
}

void GE::Sprite::fadeIn(double speed)
{
	_fade_in = true;
	_fade_speed = speed;
}

void GE::Sprite::fadeOut(double speed)
{
	_fade_out = true;
	_fade_speed = speed;
}

void GE::Sprite::setPosition(double x, double y)
{
	_position_x = x;
	_position_y = y;
}

void GE::Sprite::setPositionX(double x)
{
	_position_x = x;
}

void GE::Sprite::setPositionY(double y)
{
	_position_y = y;
}

void GE::Sprite::moveHorizontal(double velocity)
{
	_position_x += velocity * _dt;
}

void GE::Sprite::moveVertical(double velocity)
{
	_position_y += velocity * _dt;
}

//draw sprite
/**
 * @brief
 * @param x position x to draw on screen
 * @param y position y to draw on screen
 * @param pos image clip position of sprite to draw on screen
 */
void GE::Sprite::draw(double dt)
{
	//render quad
	SDL_Rect renderQuad = {(int)_position_x, (int)_position_y, _rect.w, _rect.h};

	_dt = dt;

	if (_fade_in)
	{
		setAlpha(_alpha);
		if (_alpha + (_fade_speed * dt) >= 255.0)
		{
			_alpha = 255.0;
			_fade_in = false;
		}
		_alpha += _fade_speed * dt;
	}
	if (_fade_out)
	{
		setAlpha(_alpha);
		if (_alpha - _fade_speed * dt <= 0.0)
		{
			_alpha = 0.0;
			_fade_out = false;
		}
		_alpha -= _fade_speed * dt;
	}
	//render to screen
	SDL_RenderCopy(_renderer, _texture, &_rect, &renderQuad);
}

bool GE::Sprite::isFading()
{
	return (_fade_in || _fade_out);
}

void GE::Sprite::createAnimation(std::string animation_key, SDL_Rect frame_pos, double frame_size[2])
{
	int num_of_frame = (frame_pos.w - frame_pos.x) / frame_size[0];

	_animation_data anim;
	anim._frame_pos = frame_pos;
	anim._frame_size[0] = frame_size[0];
	anim._frame_size[1] = frame_size[1];
	anim._num_of_frame = num_of_frame;

	_animation.insert(std::pair<std::string, _animation_data>(animation_key, anim));
}

void GE::Sprite::setAnimation(std::string animation_key, double speed)
{
	if (!_animation_is_playing)
		return;

	_animation_data anim = _animation.at(animation_key);

	if (_animation_frame_index >= anim._num_of_frame)
		_animation_frame_index = 0;

	_rect.x = anim._frame_pos.x + (anim._frame_size[0] * _animation_frame_index);
	_rect.y = anim._frame_pos.y;
	_rect.w = anim._frame_size[0];
	_rect.h = anim._frame_size[1];

	_animation_speed_counter += _dt;

	if (_animation_speed_counter >= speed)
	{
		_animation_speed_counter = 0;
		_animation_frame_index += 1;
	}
}

void GE::Sprite::resumeAnimation()
{
	_animation_is_playing = true;
}

void GE::Sprite::stopAnimation()
{
	_animation_is_playing = false;
}