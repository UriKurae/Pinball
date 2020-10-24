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
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	pinballMap = App->textures->Load("pinball/Map.png");


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

	MapChain();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MapChain()
{
	bodyReturn* chain;

	int Map[50] = {
	33, 202,
	27, 184,
	21, 169,
	17, 153,
	14, 132,
	13, 118,
	12, 103,
	12, 86,
	16, 68,
	23, 52,
	35, 38,
	49, 30,
	65, 24,
	80, 21,
	98, 19,
	112, 19,
	125, 19,
	135, 19,
	145, 20,
	155, 22,
	163, 25,
	170, 31,
	177, 39,
	184, 52,
	189, 66
	};

	App->physics->createChain(0, 0, Map, 50, 's');

	int Map2[104] = {
	11, 205,
	5, 191,
	0, 175,
	0, 159,
	0, 140,
	0, 125,
	0, 111,
	0, 93,
	0, 73,
	0, 56,
	0, 48,
	6, 38,
	15, 30,
	25, 22,
	35, 15,
	45, 11,
	56, 7,
	67, 4,
	78, 2,
	87, 0,
	98, 0,
	112, 0,
	120, 0,
	130, 0,
	142, 0,
	153, 0,
	166, 1,
	175, 3,
	186, 6,
	197, 9,
	208, 14,
	218, 19,
	227, 25,
	233, 30,
	239, 37,
	246, 46,
	250, 56,
	252, 66,
	253, 77,
	253, 87,
	254, 99,
	254, 108,
	254, 116,
	255, 124,
	254, 135,
	253, 146,
	251, 156,
	248, 169,
	245, 180,
	242, 190,
	239, 202,
	234, 210
	};

	App->physics->createChain(0, 0, Map2, 104, 's');

	int Map3[38] = {
	205, 59,
	204, 51,
	201, 44,
	198, 37,
	196, 31,
	205, 33,
	214, 40,
	220, 47,
	226, 55,
	231, 65,
	233, 73,
	235, 85,
	236, 100,
	235, 116,
	233, 135,
	230, 154,
	225, 173,
	220, 192,
	213, 205
	};

	App->physics->createChain(0, 0, Map3, 38, 's');
}