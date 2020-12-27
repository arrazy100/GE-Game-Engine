#include "Box2D.h"

GE::Box2D::Box2D(b2World* world, float x, float y, float w, float h, bool is_static)
{
    if (is_static)
    {
        _body_def.position.Set(pixelToMeter(x + w / 2), pixelToMeter(y));
        _body = world->CreateBody(&_body_def);

        b2PolygonShape polygon_shape;
        polygon_shape.SetAsBox(pixelToMeter(w) / 2, pixelToMeter(h) / 2);

        _body->CreateFixture(&polygon_shape, 0.0f);
        _body->SetFixedRotation(true);
    }
    else if (!is_static)
    {
        _body_def.type = b2_dynamicBody;
        _body_def.position.Set(pixelToMeter(x + w / 2), pixelToMeter(y));
        _body = world->CreateBody(&_body_def);

        b2PolygonShape polygon_shape;
        polygon_shape.SetAsBox(pixelToMeter(w) / 2, pixelToMeter(h) / 2);

        _fixture_def = new b2FixtureDef;
        _fixture_def->shape = &polygon_shape;
        _fixture_def->density = 1.0f;
        _fixture_def->friction = 0.f;
        _fixture_def->restitution = 0.0f;

        _body->CreateFixture(_fixture_def);
        _body->SetFixedRotation(true);
    }
    _w = w;
    _h = h;
}

GE::Box2D::~Box2D()
{
    delete(_fixture_def);
    _body = NULL;
    _fixture_def = NULL;
}

b2Body* GE::Box2D::getBody()
{
    return _body;
}

float GE::Box2D::getPositionX()
{
    return meterToPixel(_body->GetPosition().x) - _w / 2;
}

float GE::Box2D::getPositionY()
{
    return meterToPixel(_body->GetPosition().y);
}

void GE::Box2D::setPosition(float x, float y)
{
    _body->SetTransform(b2Vec2(meterToPixel(x), meterToPixel(y)), _body->GetAngle());
}

void GE::Box2D::moveHorizontal(float velocity)
{
    b2Vec2 vel = _body->GetLinearVelocity();
    float desiredVel = velocity;
    float velChange = desiredVel - vel.x;
    float impulse = _body->GetMass() * velChange;
    _body->ApplyLinearImpulse(b2Vec2(impulse, 0), _body->GetWorldCenter(), true);
}

void GE::Box2D::moveVertical(float velocity)
{
    _body->SetLinearVelocity(b2Vec2(0, velocity));
}

void GE::Box2D::stopMoveHorizontal()
{
    b2Vec2 vel = _body->GetLinearVelocity();
    vel.x = 0;
    _body->SetLinearVelocity(vel);
}

void GE::Box2D::applyJump(float velocity)
{
    float impulse = _body->GetMass() * velocity;
	_body->ApplyLinearImpulse(b2Vec2(0, impulse), _body->GetWorldCenter(), true);
}

float GE::Box2D::pixelToMeter(float pixel)
{
    return pixel / PPM;
}

float GE::Box2D::meterToPixel(float meter)
{
    return round(meter * PPM);
}