#ifndef GE_BOX2D_H
#define GE_BOX2D_H

#include <string>
#include <box2d/box2d.h>

#include "Sprite.h"

namespace GE
{
	class Box2D
	{
	private:
        b2BodyDef _body_def;
		b2Body* _body = NULL;
		b2FixtureDef* _fixture_def = NULL;
		const float PPM = 32.f;
		float _w = 0;
		float _h = 0;
	public:
		Box2D(b2World* world, float x, float y, float w, float h, bool is_static);
		~Box2D();
		b2Body* getBody();
		float getPositionX();
		float getPositionY();
		void setPosition(float x, float y);
		void moveHorizontal(float velocity);
		void moveVertical(float velocity);
		void stopMoveHorizontal();
		void applyJump(float velocity);
		float pixelToMeter(float pixel);
		float meterToPixel(float meter);
	};
	class Box2DListener : public b2ContactListener
	{
		private:
			int _touch_foot = 0;
		public:
			void BeginContact(b2Contact* contact);
			void EndContact(b2Contact* contact);
			int getTouchedFoot();
	};
} // namespace GE

#endif