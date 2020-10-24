#pragma once
#include "Module.h"
#include "Globals.h"


#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


class b2World;
class b2Body;
class b2Vec2;


// TODO 6: Create a small class that keeps a pointer to the b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class
class bodyReturn
{
public:
	bodyReturn():bodyPointer(nullptr), width(0), height(0){}
	bodyReturn(b2Body* body) : bodyPointer(body){}

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

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	bodyReturn* createCircle(float posX, float posY, float rad);
	bodyReturn* createRectangle(float posX, float posY, float width, float height);
	bodyReturn* createChains();

private:

	bool debug;
	b2World* world;
};