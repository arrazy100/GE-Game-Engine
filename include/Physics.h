#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Sprite.h"
#include "Shape.h"

namespace GE
{
	class Physics
	{
	private:
		GE::Sprite* _body_sprite = NULL;
		GE::Shape* _body_shape = NULL;
		SDL_Rect _rect;
		double _gravity = 0;
		double _xvelocity = 0;
		double _yvelocity = 0;
		bool _is_static = false;

	public:
		Physics(bool is_static);
		void setBody(GE::Sprite* body);
		void setBody(GE::Shape* body);
		void setBody(SDL_Rect rect);
		void update();
		void setGravity(double gravity);
		void setRect(std::string rectPoint, double position);
		void setVelocityX(double xvelocity);
		void setVelocityY(double yvelocity);
		bool isStatic();
		double getBodyCoordinateX();
		double getBodyCoordinateY();
		double getGravity();
		double getRect(std::string rectPoint);
		double getVelocityX();
		double getVelocityY();
		SDL_Rect getRect();
		std::string detectAABB(GE::Physics* b);
		std::string detectMultipleAABB(std::initializer_list<GE::Physics> PhysicsObject);
	};
} // namespace GE

#endif