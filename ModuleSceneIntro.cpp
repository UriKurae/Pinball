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

	// Creation and setup of the ball

	 ball = App->physics->createCircle(454.0f, 731.0f, 13.0f);
	 


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
	
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ball->bodyPointer->ApplyForce(b2Vec2(0, 2000), b2Vec2(ball->getPosition().x, ball->getPosition().y), 1);
	}


	App->renderer->Blit(pinballMap, 0, 0, NULL);
	App->renderer->Blit(ballTexture, (ball->getPosition().x - ball->width), ball->getPosition().y - ball->width, NULL);

	MapChain();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MapChain()
{
	PhysBody* chain;

	// Pivot 0, 0
	int Map[206] = {
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


	App->physics->createChain(0, 0, Map, 206);
}