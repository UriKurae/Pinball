#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

// Create Circles
bodyReturn* ModulePhysics::createCircle(float posX, float posY, float rad)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	//body.angle = 1.0f;
	float radius = PIXEL_TO_METERS(rad);
	body.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fixture;
	fixture.density = 1.0f;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);
	bodyReturn* circleBody = new bodyReturn();
	circleBody->bodyPointer = b;
	circleBody->height = METERS_TO_PIXELS(radius);
	circleBody->width = METERS_TO_PIXELS(radius);

	return circleBody;
}

// Create Rectangles
bodyReturn* ModulePhysics::createRectangle(float posX, float posY, float width, float height)
{
	
	b2BodyDef boxBody;
	boxBody.type = b2_dynamicBody;
	boxBody.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));

	b2Body* b = world->CreateBody(&boxBody);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(width, height);

	b2FixtureDef boxFixture;
	boxFixture.density = 1.0f;
	boxFixture.friction = 0.3f;
	boxFixture.shape = &boxShape;

	b->CreateFixture(&boxFixture);

	bodyReturn* rectangleBody = new bodyReturn();
	rectangleBody->bodyPointer = b;
	rectangleBody->height = METERS_TO_PIXELS(width);
	rectangleBody->width = METERS_TO_PIXELS(height);

	return rectangleBody;

	
}

//Create Chains
bodyReturn* ModulePhysics::createChains()
{
	
	b2Vec2 vertices[49];
	int points[98] =
	{
		43, 38,
		43, 31,
		43, 20,
		42, 11,
		42, 1,
		50, 7,
		56, 11,
		64, 19,
		68, 23,
		75, 32,
		80, 24,
		82, 18,
		88, 4,
		91, 13,
		93, 25,
		95, 40,
		102, 38,
		112, 35,
		108, 45,
		103, 60,
		106, 64,
		112, 66,
		117, 67,
		109, 74,
		110, 83,
		108, 89,
		104, 93,
		111, 100,
		103, 105,
		102, 110,
		100, 116,
		106, 121,
		103, 125,
		99, 128,
		96, 136,
		88, 145,
		80, 149,
		65, 148,
		54, 144,
		46, 133,
		35, 137,
		40, 126,
		25, 125,
		31, 115,
		10, 104,
		29, 92,
		1, 77,
		29, 63,
		15, 38
	};

	int counter = 0;
	for (int i = 0; i < 49; ++i)
	{
		vertices[i].Set(PIXEL_TO_METERS(points[counter]), PIXEL_TO_METERS(points[counter + 1]));
		counter += 2;
	}


	b2BodyDef chainBody;
	chainBody.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	b2Body* chainB = world->CreateBody(&chainBody);


	b2ChainShape chain;
	chain.CreateLoop(vertices, 49);

	b2FixtureDef chainFixture;
	chainFixture.shape = &chain;

	chainB->CreateFixture(&chainFixture);
	
	bodyReturn* chainBodyPointer = new bodyReturn();
	chainBodyPointer->bodyPointer = chainB;
	chainBodyPointer->getRotation();
	return chainBodyPointer;

	
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}


// 
update_status ModulePhysics::PostUpdate()
{

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

b2Vec2 bodyReturn::getPosition()
{
	b2Vec2 pos;

	pos = bodyPointer->GetPosition();

	pos.x = METERS_TO_PIXELS(pos.x);
	pos.y = METERS_TO_PIXELS(pos.y);
	
	
	return pos;
	
}

double bodyReturn::getRotation()
{	
	double rot = (double)(RADTODEG * bodyPointer->GetTransform().q.GetAngle());

	return rot;
}
