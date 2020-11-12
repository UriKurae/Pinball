#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"
//#include "Box2D/Box2D/Box2D.h"

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
PhysBody* ModulePhysics::createCircle(float posX, float posY, float rad, std::string tag, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;

	float radius = PIXEL_TO_METERS(rad);
	body.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fixture;
	fixture.density = 0.2f;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);
	PhysBody* circleBody = new PhysBody();
	circleBody->bodyPointer = b;
	circleBody->bodyTag = tag;
	b->SetUserData(circleBody);
	circleBody->height = METERS_TO_PIXELS(radius);
	circleBody->width = METERS_TO_PIXELS(radius);

	return circleBody;
}

// Create Rectangles
PhysBody* ModulePhysics::createRectangle(float posX, float posY, float width, float height, b2BodyType type)
{
	
	b2BodyDef boxBody;
	boxBody.type = type;
	boxBody.position.Set(PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY));


	b2Body* b = world->CreateBody(&boxBody);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(width, height);

	b2FixtureDef boxFixture;
	boxFixture.density = 0.5f;
	boxFixture.friction = 0.3f;
	boxFixture.shape = &boxShape;


	b->CreateFixture(&boxFixture);

	PhysBody* rectangleBody = new PhysBody();
	rectangleBody->bodyPointer = b;
	rectangleBody->height = METERS_TO_PIXELS(width);
	rectangleBody->width = METERS_TO_PIXELS(height);

	return rectangleBody;

	
}

//Create Chains
PhysBody* ModulePhysics::createChain(int x, int y, int* arr, int num, b2BodyType type)
{
	b2BodyDef body;

	body.type = type;
	

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;

	b2Vec2* vertix = new b2Vec2[num / 2];

	for (int i = 0; i < (num / 2); i++)
	{
		vertix[i] = { PIXEL_TO_METERS(arr[i * 2]), PIXEL_TO_METERS(arr[i * 2 + 1]) };
	}

	shape.CreateLoop(vertix, num / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0;

	b->CreateFixture(&fixture);

	PhysBody* pBody = new PhysBody();
	pBody->bodyPointer = b;

	return pBody;
}

b2World* ModulePhysics::GetWorld()
{
	return world;
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	world->SetContactListener(this);
	
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

b2Vec2 PhysBody::getPosition()
{
	b2Vec2 pos;

	pos = bodyPointer->GetPosition();

	pos.x = METERS_TO_PIXELS(pos.x);
	pos.y = METERS_TO_PIXELS(pos.y);
	
	
	return pos;
	
}

double PhysBody::getRotation()
{	
	double rot = (double)(RADTODEG * bodyPointer->GetTransform().q.GetAngle());

	return rot;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();


	if (physA && physA->listener != NULL)
	{
		physA->listener->OnCollision(physA, physB);

		collisionWithBumper(physA, physB);

	}

	if (physB && physB->listener != NULL)
	{
		physB->listener->OnCollision(physB, physA);
		
		collisionWithBumper(physB, physA);
	}

	if (physA && physA->listener != NULL)
	{
		physA->listener->OnCollision(physA, physB);

		collisionWithCanyon(physA, physB);

	}

	if (physB && physB->listener != NULL)
	{
		physB->listener->OnCollision(physB, physA);

		collisionWithCanyon(physB, physA);
	}
}

void ModulePhysics::collisionWithBumper(PhysBody* body1, PhysBody* body2)
{
	if (body1 != NULL && body2 != NULL)
	{
		if (body1->bodyTag != "" && body2->bodyTag != "")
		{
			if (body1->bodyTag == "Player" && body2->bodyTag == "SmallBumper")
			{
				App->player->addPoint(200);
			}
			else if (body2->bodyTag == "Player" && body1->bodyTag == "SmallBumper")
			{
				App->player->addPoint(200);
			}
			else if (body1->bodyTag == "Player" && body2->bodyTag == "BigBumper")
			{
				App->player->addPoint(50);
			}
			else if (body2->bodyTag == "Player" && body1->bodyTag == "BigBumper")
			{
				App->player->addPoint(50);
			}
		}
	}

}

void ModulePhysics::collisionWithCanyon(PhysBody* body1, PhysBody* body2)
{

	if (body1 != NULL && body2 != NULL)
	{
		if (body1->bodyTag != "" && body2->bodyTag != "")
		{
			if (body1->bodyTag == "Player" && body2->bodyTag == "canyon")
			{
				App->player->addPoint(200);
				App->player->setStunTime(1000.0f);
			}
			else if (body2->bodyTag == "Player" && body1->bodyTag == "canyon")
			{
				App->player->addPoint(200);
				App->player->setStunTime(1000.0f);

			}
		}
	}

}