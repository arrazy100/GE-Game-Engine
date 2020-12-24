#ifndef SHAPE_H
#define SHAPE_H

#include <SDL2/SDL.h>

namespace GE
{
	class Shape
	{
	private:
		//init object
		SDL_Renderer *_renderer = NULL;
		SDL_Rect _rect = {0, 0, 0, 0};

	public:
		//passing init object to get current renderer
		Shape(SDL_Renderer *renderer);
		~Shape();
		SDL_Rect getRect();
		//draw rectangle
		void drawRectangle(double x, double y, double size[2], SDL_Color c);
		//draw ellips
		void drawEllips(double x, double y, double radius, SDL_Color c);
		//draw triangle
		void drawTriangle(double x, double y, double size[3], SDL_Color c);
	};
} // namespace GE

#endif