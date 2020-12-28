#include "Box2D.h"

struct UserData
{
    std::string name;
};

GE::Box2D::Box2D(b2World* world, float x, float y, float w, float h, bool is_static, bool is_sensor, std::string user_data)
{
    _world = world;
    _w = w;
    _h = h;
    UserData* body_data = new UserData;
    body_data->name = user_data;
    
    if (is_static)
    {
        b2BodyDef body_def;
        body_def.position.Set(pixelToMeter(x + w / 2), pixelToMeter(y));
        _body = world->CreateBody(&body_def);
        _body->GetUserData().pointer = reinterpret_cast<uintptr_t>(body_data);

        b2PolygonShape polygon_shape;
        polygon_shape.SetAsBox(pixelToMeter(w) / 2, pixelToMeter(h) / 2);

        _body->CreateFixture(&polygon_shape, 0.0f)->SetSensor(is_sensor);
        _body->SetFixedRotation(true);
    }
    else if (!is_static)
    {
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position.Set(pixelToMeter(x + w / 2), pixelToMeter(y));
        _body = world->CreateBody(&body_def);
        _body->GetUserData().pointer = reinterpret_cast<uintptr_t>(body_data);
        _body->SetFixedRotation(true);

        b2PolygonShape polygon_shape;

        polygon_shape.SetAsBox(pixelToMeter(w) / 2, pixelToMeter(h) / 2);
        _fixture_def = new b2FixtureDef;
        _fixture_def->shape = &polygon_shape;
        _fixture_def->density = 1.0f;
        _fixture_def->friction = 0.f;
        _fixture_def->restitution = 0.0f;
        _fixture_def->isSensor = is_sensor;
        _body->CreateFixture(_fixture_def);
        
        polygon_shape.SetAsBox(pixelToMeter(w) / 2, 0.1, b2Vec2(0, pixelToMeter(h) / 2), 0);
        UserData* data = new UserData;
        data->name = "bottom";
        _fixture_def->shape = &polygon_shape;
        _fixture_def->isSensor = true;
        _fixture_def->userData.pointer = reinterpret_cast<uintptr_t>(data);
        _body->CreateFixture(_fixture_def);
        
        polygon_shape.SetAsBox(pixelToMeter(w) / 2, 0.1, b2Vec2(0, -pixelToMeter(h) / 2), 0);
        UserData* data2 = new UserData;
        data2->name = "top";
        _fixture_def->shape = &polygon_shape;
        _fixture_def->isSensor = true;
        _fixture_def->userData.pointer = reinterpret_cast<uintptr_t>(data2);
        _body->CreateFixture(_fixture_def);

        polygon_shape.SetAsBox(0.1, pixelToMeter(h) / 3, b2Vec2(-pixelToMeter(w) / 2, 0), 0);
        UserData* data3 = new UserData;
        data3->name = "left";
        _fixture_def->shape = &polygon_shape;
        _fixture_def->isSensor = true;
        _fixture_def->userData.pointer = reinterpret_cast<uintptr_t>(data3);
        _body->CreateFixture(_fixture_def);

        polygon_shape.SetAsBox(0.1, pixelToMeter(h) / 3, b2Vec2(pixelToMeter(w) / 2, 0), 0);
        UserData* data4 = new UserData;
        data4->name = "right";
        _fixture_def->shape = &polygon_shape;
        _fixture_def->isSensor = true;
        _fixture_def->userData.pointer = reinterpret_cast<uintptr_t>(data4);
        _body->CreateFixture(_fixture_def);
    }
}

GE::Box2D::~Box2D()
{
    _world = NULL;
    _body = NULL;
    delete(_fixture_def);
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

void GE::Box2DListener::BeginContact(b2Contact* contact)
{
    UserData* A = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetUserData().pointer);
    if (A && A->name == "top")
        _touch_top++;
    else if (A && A->name == "bottom")
        _touch_bottom++;
    else if (A && A->name == "right")
        _touch_right++;
    else if (A && A->name == "left")
        _touch_left++;

    UserData* AA = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    UserData* BB = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (AA && AA->name == "coin" && BB && BB->name == "npc")
    {
        _coin++;
    }
    else if (AA && AA->name == "npc" && BB && BB->name == "coin")
    {
        _coin++;
    }

    UserData* B = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetUserData().pointer);
    if (B && B->name == "top")
        _touch_top++;
    else if (B && B->name == "bottom")
        _touch_bottom++;
    else if (B && B->name == "right")
        _touch_right++;
    else if (B && B->name == "left")
        _touch_left++;
}

void GE::Box2DListener::EndContact(b2Contact* contact)
{
    UserData* A = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetUserData().pointer);
    if (A && A->name == "top")
        _touch_top--;
    else if (A && A->name == "bottom")
        _touch_bottom--;
    else if (A && A->name == "right")
        _touch_right--;
    else if (A && A->name == "left")
        _touch_left--;

   UserData* B = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetUserData().pointer);
    if (B && B->name == "top")
        _touch_top--;
    else if (B && B->name == "bottom")
        _touch_bottom--;
    else if (B && B->name == "right")
        _touch_right--;
    else if (B && B->name == "left")
        _touch_left--;
}

int GE::Box2DListener::getTouchedTop()
{
    return _touch_top;
}

int GE::Box2DListener::getTouchedBottom()
{
    return _touch_bottom;
}

int GE::Box2DListener::getTouchedRight()
{
    return _touch_right;
}

int GE::Box2DListener::getTouchedLeft()
{
    return _touch_left;
}

int GE::Box2DListener::getCoin()
{
    return _coin;
}