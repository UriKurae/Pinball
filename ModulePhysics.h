#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include <string>
#include "ModulePlayer.h"


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
	std::string bodyTag;

	int typeSensor = 0;

	b2Vec2 getPosition();
	double getRotation();

	bool Contains(int x, int y) const;



	Module* listener;
};

class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* createCircle(float posX, float posY, float rad, int typeSensor, std::string bodyTag = "", b2BodyType type = b2BodyType::b2_dynamicBody);
	PhysBody* createRectangle(float posX, float posY, float width, float height, int typeSensor, b2BodyType type = b2BodyType::b2_staticBody);
	PhysBody* createChain(int x, int y, int* arr, int num, b2BodyType type = b2BodyType::b2_staticBody, float restitution = 0.0f);
	b2World* GetWorld();

	void BeginContact(b2Contact* contact);

private:
	
	PhysBody* pb;
	bool flag = false;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

	bool debug;
	b2World* world;
	b2ContactListener listener;
	uint bumpers;
	uint LooseBall;
	uint Spring;
	uint GameOver;


	void collisionWithDead(PhysBody* body1, PhysBody* body2);
	void collisionWithBumper(PhysBody* body1, PhysBody* body2);
	void collisionWithCanyon(PhysBody* body1, PhysBody* body2);

};