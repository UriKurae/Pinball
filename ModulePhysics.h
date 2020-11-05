#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"


#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


class b2World;
class b2Body;
class b2Vec2;
enum b2BodyType;


class PhysBody
{
public:
	PhysBody():bodyPointer(nullptr), width(0), height(0){}
	PhysBody(b2Body* body) : bodyPointer(body){}

	b2Body* bodyPointer = nullptr;

	float width = 0;
	float height = 0;
	float rotation = 0;

	b2Vec2 getPosition();
	double getRotation();


};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	
	PhysBody* createCircle(float posX, float posY, float rad);
	PhysBody* createRectangle(float posX, float posY, float width, float height);
	PhysBody* createChain(int x, int y, int* arr, int num, b2BodyType type = b2BodyType::b2_staticBody);

private:

	bool debug;
	b2World* world;
};