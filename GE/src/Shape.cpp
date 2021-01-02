#include <Shape.h>

/**
 * @brief
 * @param init passing GE::Init object to get current renderer
 */
GE::Shape::Shape(SDL_Renderer* renderer) : _renderer(renderer)
{
}

/**
 * @brief
 * class destructor
 */
GE::Shape::~Shape()
{
}

SDL_Rect GE::Shape::getRect()
{
	return _rect;
}

/**
 * @brief
 * @param x position x to draw on screen
 * @param y position y to draw on screen
 * @param size initializer_list with parameter { int width, int height }
 * @param c the rgba color of rectangle
 */
void GE::Shape::drawRectangle(double x, double y, double size[2], SDL_Color c)
{
	SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a); //color of rectangle
	_rect = {(int)x, (int)y, (int)size[0], (int)size[1]};
	SDL_RenderFillRect(_renderer, &_rect);
}

/**
 * @brief
 * @param x position x to draw on screen
 * @param y position y to draw on screen
 * @param radius the radius of ellips
 * @param c the rgba color of ellips
 */
void GE::Shape::drawEllips(double x, double y, double radius, SDL_Color c)
{
	SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a); //color of ellips
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w;
			int dy = radius - h;
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(_renderer, x + radius + dx, y + radius + dy);
			}
		}
	}
}

/**
 * @brief
 * @param x position x to draw on screen
 * @param y position y to draw on screen
 * @param size initializer_list with parameter { int a, int b, int c }
 * @param c the rgba color of triangle
 */
void GE::Shape::drawTriangle(double x, double y, double size[3], SDL_Color c)
{
	SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a); //color of triangle
	//draw triangle line
	SDL_RenderDrawLine(_renderer, x, y, x, y + size[0]);
	SDL_RenderDrawLine(_renderer, x, y + size[0], x + size[2], y + size[2]);
	SDL_RenderDrawLine(_renderer, x, y, x + size[2], y + size[2]);
}