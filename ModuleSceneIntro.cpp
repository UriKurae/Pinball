#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ballTexture = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ballTexture = App->textures->Load("pinball/Ball.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	pinballMap = App->textures->Load("pinball/Map.png");

	// Creation and setup of the ball. 

	 ball = App->physics->createCircle(454.0f, 731.0f, 13.0f);
	 
	 // Creation and setup of the levers
	 CreateLevers();
	 MapChain();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && launched == false)
	{
		ball->bodyPointer->ApplyForce(b2Vec2(0, -50), b2Vec2(ball->getPosition().x, ball->getPosition().y), 1);
		launched = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		leverJointB->SetMotorSpeed(-10.0f);
	
	}
	else
	{
		leverJointB->SetMotorSpeed(10.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		leverJointA->SetMotorSpeed(10.0f);
	}
	else
	{
		leverJointA->SetMotorSpeed(-10.0f);
	}

	App->renderer->Blit(pinballMap, 0, 0, NULL);
	App->renderer->Blit(ballTexture, (ball->getPosition().x - ball->width), ball->getPosition().y - ball->width, NULL);



	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MapChain()
{

	// Map outside boundaries
	int MapBorder[206] = {
		455, 765,
		468, 760,
		472, 740,
		472, 687,
		469, 569,
		472, 413,
		473, 287,
		474, 162,
		472, 119,
		460, 92,
		444, 64,
		416, 39,
		377, 19,
		332, 6,
		285, 0,
		228, 0,
		151, 0,
		114, 10,
		75, 22,
		46, 37,
		20, 64,
		0, 89,
		1, 322,
		24, 387,
		39, 416,
		44, 430,
		30, 443,
		31, 496,
		31, 524,
		38, 533,
		26, 550,
		23, 577,
		20, 611,
		19, 640,
		19, 679,
		20, 711,
		20, 743,
		19, 764,
		24, 776,
		37, 780,
		49, 780,
		59, 776,
		64, 755,
		62, 732,
		64, 720,
		84, 730,
		107, 743,
		142, 758,
		171, 772,
		171, 801,
		1, 800,
		0, 562,
		0, 360,
		-6, 305,
		-4, 226,
		-6, 100,
		-6, 72,
		20, 41,
		60, 15,
		103, 2,
		140, -8,
		216, -12,
		279, -12,
		328, -6,
		370, 3,
		401, 16,
		432, 34,
		461, 63,
		481, 108,
		483, 152,
		482, 188,
		481, 345,
		481, 507,
		481, 604,
		481, 736,
		480, 764,
		472, 798,
		416, 800,
		345, 800,
		276, 797,
		276, 774,
		313, 754,
		369, 726,
		386, 718,
		385, 772,
		390, 780,
		410, 781,
		426, 776,
		427, 720,
		427, 626,
		422, 573,
		417, 548,
		404, 532,
		428, 474,
		421, 464,
		436, 450,
		443, 444,
		439, 568,
		436, 580,
		435, 621,
		435, 705,
		437, 755,
		445, 764
	};
	App->physics->createChain(0, 0, MapBorder, 206);





}

void ModuleSceneIntro::CreateLevers()
{
	anchorPointA = App->physics->createRectangle(150, 710, 0.1f, 0.1f );
	leverA = App->physics->createRectangle(174, 710, 0.65f, 0.1f, b2BodyType::b2_dynamicBody);
	
	

	anchorPointB = App->physics->createRectangle(294, 710, 0.1f, 0.1f);
	leverB = App->physics->createRectangle(270, 710, 0.65f, 0.1f, b2BodyType::b2_dynamicBody);


	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.Initialize(leverA->bodyPointer, anchorPointA->bodyPointer, anchorPointA->bodyPointer->GetWorldCenter());
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.lowerAngle = DEGTORAD * -20;
	revoluteJointDef.upperAngle = DEGTORAD * 45;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.maxMotorTorque = 50.0f;
	revoluteJointDef.motorSpeed = 0.0f;
	revoluteJointDef.enableMotor = true;
	leverJointA =(b2RevoluteJoint*) App->physics->GetWorld()->CreateJoint(&revoluteJointDef);

	revoluteJointDef.Initialize(leverB->bodyPointer, anchorPointB->bodyPointer, anchorPointB->bodyPointer->GetWorldCenter());
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.lowerAngle = DEGTORAD * -45;
	revoluteJointDef.upperAngle = DEGTORAD * 20;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.maxMotorTorque = 50.0f;
	revoluteJointDef.motorSpeed = 0.0f;
	revoluteJointDef.enableMotor = true;
	leverJointB = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&revoluteJointDef);

	
}
