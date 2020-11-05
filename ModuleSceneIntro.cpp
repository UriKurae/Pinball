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

	 ball = App->physics->createCircle(490.0f, 700.0f, 13.0f);
	 


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
	
	
	App->renderer->Blit(pinballMap, 0, 0, NULL);
	App->renderer->Blit(ballTexture, ball->getPosition().x, ball->getPosition().y, NULL);

	MapChain();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MapChain()
{
	PhysBody* chain;

	// Pivot 0, 0
	int Map[226] = {
		251, 413,
		254, 390,
		253, 311,
		255, 237,
		254, 130,
		253, 82,
		250, 56,
		236, 32,
		224, 22,
		205, 13,
		161, 1,
		133, 0,
		91, 0,
		62, 3,
		41, 10,
		20, 22,
		6, 36,
		-1, 49,
		0, 172,
		9, 200,
		18, 218,
		22, 230,
		16, 236,
		15, 251,
		15, 270,
		16, 283,
		17, 288,
		12, 303,
		11, 322,
		11, 342,
		11, 366,
		11, 391,
		12, 412,
		15, 419,
		23, 420,
		30, 418,
		34, 411,
		33, 393,
		34, 388,
		47, 394,
		60, 401,
		79, 410,
		91, 415,
		91, 431,
		54, 431,
		24, 431,
		1, 431,
		0, 196,
		-4, 179,
		-5, 162,
		-3, 132,
		-3, 96,
		-4, 63,
		-5, 42,
		11, 20,
		27, 6,
		51, -3,
		74, -6,
		98, -8,
		129, -8,
		157, -6,
		181, -6,
		205, 2,
		230, 14,
		245, 33,
		254, 51,
		257, 66,
		258, 95,
		258, 115,
		260, 133,
		260, 151,
		260, 176,
		260, 222,
		260, 251,
		261, 271,
		260, 318,
		261, 349,
		261, 405,
		260, 423,
		255, 430,
		242, 430,
		221, 431,
		187, 431,
		163, 430,
		147, 430,
		147, 415,
		166, 406,
		186, 397,
		206, 387,
		208, 392,
		207, 410,
		207, 417,
		212, 419,
		221, 420,
		229, 418,
		230, 398,
		229, 374,
		229, 346,
		228, 320,
		224, 298,
		216, 288,
		226, 265,
		229, 257,
		224, 250,
		233, 243,
		236, 259,
		234, 283,
		235, 309,
		234, 332,
		233, 353,
		234, 377,
		235, 403,
		237, 411
	};

	App->physics->createChain(0, 0, Map, 226);
}