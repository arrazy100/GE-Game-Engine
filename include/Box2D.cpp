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
        polygon_shape.SetAsBox(pixelToMeter(w) / 2, pixelToMeter(h) / 2, b2Vec2(0, pixelToMeter(h / 2)), 0);

        _fixture_def = new b2FixtureDef;
        _fixture_def->shape = &polygon_shape;
        _fixture_def->density = 0.0f;
        _fixture_def->friction = 0.0f;
        _fixture_def->isSensor = is_sensor;

        _body->CreateFixture(_fixture_def);
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
        UserData* fixture_data = new UserData;
        fixture_data->name = user_data;

        polygon_shape.SetAsBox(pixelToMeter(w) / 2, pixelToMeter(h) / 2, b2Vec2(0, pixelToMeter(h / 2)), 0);
        _fixture_def = new b2FixtureDef;
        _fixture_def->shape = &polygon_shape;
        _fixture_def->density = 1.0f;
        _fixture_def->friction = 0.0f;
        _fixture_def->restitution = 0.0f;
        _fixture_def->isSensor = is_sensor;
        _fixture_def->userData.pointer = reinterpret_cast<uintptr_t>(fixture_data);
        _body->CreateFixture(_fixture_def);
    }
}

GE::Box2D::~Box2D()
{
    for (b2Fixture* f = _body->GetFixtureList(); f; f = f->GetNext())
    {
        delete(reinterpret_cast<UserData*>(f->GetUserData().pointer));
    }
    delete(reinterpret_cast<UserData*>(_body->GetUserData().pointer));
    _world->DestroyBody(_body);
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
    _body->SetTransform(b2Vec2(meterToPixel(x + _w / 2), meterToPixel(y / 2)), _body->GetAngle());
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

void GE::Box2D::addFixture(float w, float h, b2Vec2 origin, bool is_sensor, std::string userdata)
{
    b2PolygonShape polygon_shape;
    polygon_shape.SetAsBox(pixelToMeter(w) / 2, pixelToMeter(h) / 2, b2Vec2(pixelToMeter(origin.x), pixelToMeter(origin.y)), 0);
    UserData* data = new UserData;
    data->name = userdata;

    _fixture_def = new b2FixtureDef;
    _fixture_def->shape = &polygon_shape;
    _fixture_def->density = 1.0f;
    _fixture_def->friction = 0.0f;
    _fixture_def->restitution = 0.0f;
    _fixture_def->isSensor = is_sensor;
    _fixture_def->userData.pointer = reinterpret_cast<uintptr_t>(data);
    _body->CreateFixture(_fixture_def);
}

b2Body* GE::Box2D::touchWithBody(std::string body_name)
{
    for (b2ContactEdge* c = getBody()->GetContactList(); c; c = c->next)
    {
        if (c && c->contact->IsTouching())
        {
            UserData* A = reinterpret_cast<UserData*>(c->contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            UserData* B = reinterpret_cast<UserData*>(c->contact->GetFixtureB()->GetBody()->GetUserData().pointer);

            if (A && A->name == body_name)
            {
                return c->contact->GetFixtureA()->GetBody();
            }
            else if (B && B->name == body_name)
            {
                return c->contact->GetFixtureB()->GetBody();
            }
        }
    }
}

b2Body* GE::Box2D::touchWithFixture(std::string body_name, std::string fixture_name)
{
    for (b2ContactEdge* c = getBody()->GetContactList(); c; c = c->next)
    {
        if (c && c->contact->IsTouching())
        {
            UserData* A = reinterpret_cast<UserData*>(c->contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            UserData* B = reinterpret_cast<UserData*>(c->contact->GetFixtureB()->GetBody()->GetUserData().pointer);

            if ((A && A->name != body_name) || (B && B->name != body_name))
            {
                continue;
            }

            UserData* fixture_A = reinterpret_cast<UserData*>(c->contact->GetFixtureA()->GetUserData().pointer);
            UserData* fixture_B = reinterpret_cast<UserData*>(c->contact->GetFixtureB()->GetUserData().pointer);

            if (fixture_A && fixture_A->name == body_name)
            {
                return c->contact->GetFixtureA()->GetBody();
            }
            else if (fixture_B && fixture_B->name == body_name)
            {
                return c->contact->GetFixtureB()->GetBody();
            }
        }
    }
}

void GE::Box2D::drawFixture(SDL_Renderer* renderer, std::string fixture_name)
{
    b2Fixture* fixture = NULL;
	
	for (b2Fixture* f = _body->GetFixtureList(); f; f = f->GetNext())
	{
		UserData* F = reinterpret_cast<UserData*>(f->GetUserData().pointer);
		if (F && F->name == fixture_name)
		{
			fixture = f;
			break;
		}
	}

	b2PolygonShape* shape = (b2PolygonShape*) fixture->GetShape();

    b2Vec2 p1 = _body->GetWorldPoint(shape->m_vertices[0]);
    b2Vec2 p2 = _body->GetWorldPoint(shape->m_vertices[1]);
    b2Vec2 p3 = _body->GetWorldPoint(shape->m_vertices[2]);
    b2Vec2 p4 = _body->GetWorldPoint(shape->m_vertices[3]);

    SDL_Rect r;
    r.x = meterToPixel(p1.x);
    r.y = meterToPixel(p1.y);
    r.w = meterToPixel(p2.x - p1.x);
    r.h = meterToPixel(p3.y - p1.y);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
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