#ifndef SPRITE_H
#define SPRITE_H

#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace GE
{
	class Sprite
	{
	private:
		SDL_Renderer* _renderer = NULL;
		SDL_Texture* _texture = NULL;
		std::string _filename = "";
		SDL_Rect _rect = {0, 0, 0, 0};
		double _dt;

		double _position_x = 0;
		double _position_y = 0;

		double _alpha = 255.0;
		double _fade_speed = 0.0;
		bool _fade_in = false;
		bool _fade_out = false;

		struct _animation_data
		{
			SDL_Rect _frame_pos;
			double _frame_size[2];
			int _num_of_frame;
		};
		bool _animation_is_playing = true;
		std::map<std::string, _animation_data> _animation;
		int _animation_frame_index = 0;
		double _animation_speed_counter = 0;

	public:
		Sprite(SDL_Renderer* renderer, std::string image);
		~Sprite();
		void loadImage(std::string imagePath);
		SDL_Rect getRect();
		double getPositionX();
		double getPositionY();
		void setClip(SDL_Rect r);
		void setClip(double r[4]);
		void setAlpha(double alpha);
		void fadeIn(double speed);
		void fadeOut(double speed);
		bool isFading();
		void setPosition(double x, double y);
		void setPositionX(double x);
		void setPositionY(double y);
		void moveHorizontal(double velocity);
		void moveVertical(double velocity);
		void render(double dt);
		void createAnimation(std::string animation_key, SDL_Rect frame_pos, double frame_size[2]);
		void setAnimation(std::string animation_key, double speed);
		void resumeAnimation();
		void stopAnimation();
	};
} // namespace GE

#endif