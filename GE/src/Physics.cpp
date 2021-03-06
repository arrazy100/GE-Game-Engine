#include <Physics.h>

GE::Physics::Physics(bool is_static)
{
	_is_static = is_static;
}

void GE::Physics::setBody(GE::Sprite* body)
{
	_body_sprite = body;
}

void GE::Physics::setBody(GE::Shape* body)
{
	_body_shape = body;
}

void GE::Physics::setBody(SDL_Rect rect)
{
	_rect = rect;
}

void GE::Physics::update()
{
	if (_body_sprite)
	{
		_rect = _body_sprite->getRect();
		if (_xvelocity != 0)
		{
			_body_sprite->moveHorizontal(_xvelocity);
			_rect.x = _body_sprite->getPositionX();
		}
		if (_yvelocity != 0)
		{
			_body_sprite->moveVertical(_yvelocity);
			_rect.y = _body_sprite->getPositionY();
		}
	}
	else if (_body_shape) _rect = _body_shape->getRect();
}

void GE::Physics::setGravity(double gravity)
{
	_gravity = gravity;
}

void GE::Physics::setRect(std::string rectPoint, int position)
{
	if (rectPoint == "left")
		_rect.x = (int)position;
	else if (rectPoint == "right")
		_rect.x = (int)position - _rect.w;
	else if (rectPoint == "top")
		_rect.y = (int)position;
	else if (rectPoint == "bottom")
		_rect.y = (int)position - _rect.h;

	if (_body_sprite != NULL)
	{
		_body_sprite->setPositionX(_rect.x);
		_body_sprite->setPositionY(_rect.y);
	}
}

void GE::Physics::setVelocityX(double xvelocity)
{
	_xvelocity = xvelocity;
}

void GE::Physics::setVelocityY(double yvelocity)
{
	_yvelocity = yvelocity;
}

bool GE::Physics::isStatic()
{
	return _is_static;
}

int GE::Physics::getBodyCoordinateX()
{
	return _rect.x;
}

int GE::Physics::getBodyCoordinateY()
{
	return _rect.y;
}

double GE::Physics::getGravity()
{
	return _gravity;
}

SDL_Rect GE::Physics::getRect()
{
	return _rect;
}

int GE::Physics::getRect(std::string rectPoint)
{
	if (rectPoint == "left")
	{
		return _rect.x;
	}
	else if (rectPoint == "right")
	{
		return _rect.x + _rect.w;
	}
	else if (rectPoint == "top")
	{
		return _rect.y;
	}
	else if (rectPoint == "bottom")
	{
		return _rect.y + _rect.h;
	}

	return 0;
}

double GE::Physics::getVelocityX()
{
	return _xvelocity;
}

double GE::Physics::getVelocityY()
{
	return _yvelocity;
}

std::string GE::Physics::detectAABB(GE::Physics* b)
{
	SDL_Rect bb = b->getRect();
	// check intersection
	if (SDL_HasIntersection(&_rect, &bb))
	{
		if (getRect("right") > b->getRect("left") && getRect("left") + 1 < b->getRect("right") && getVelocityY() > 0)
		{
			setVelocityY(0);
			setRect("bottom", b->getRect("top"));
			return "top";
		}
		else if (getRect("right") > b->getRect("left") && getRect("left") + 1 < b->getRect("right") && getVelocityY() < 0)
		{
			setVelocityY(0);
			setRect("top", b->getRect("bottom"));
			return "bottom";
		}
		else
		{
			if (getVelocityX() > 0)
			{
				setVelocityX(-1);
				setRect("right", b->getRect("left") - 1);
				return "right";
			}
			else if (getVelocityX() < 0)
			{
				setVelocityX(1);
				setRect("left", b->getRect("right") + 1);
				return "left";
			}
		}
	}

	else if (_gravity > 0)
	{
		if (getRect("bottom") == b->getRect("top") &&
			(getRect("right") < b->getRect("left") || getRect("left") > b->getRect("right")))
		{
			return "fallFromTop";
		}
	}
	return "None";
}

std::string GE::Physics::detectMultipleAABB(std::initializer_list<GE::Physics> physicsObject)
{
	for (GE::Physics b : physicsObject)
	{
		SDL_Rect bb = b.getRect();
		if (SDL_HasIntersection(&_rect, &bb))
		{
			return detectAABB(&b);
		}

		// check in top wall
		if (getRect("bottom") == b.getRect("top") &&
			getRect("right") > b.getRect("left") + 1 &&
			getRect("left") + 1 < b.getRect("right"))
		{
			setVelocityY(0);
			return "top";
		}
	}
	return "None";
}