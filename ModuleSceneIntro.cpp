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


	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		bodyReturn *circle = App->physics->createCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25.0f);
		circleList.add(circle);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		bodyReturn* rectangle = App->physics->createRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 1.0f, 0.5f);
		rectangleList.add(rectangle);
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		bodyReturn *rick = App->physics->createChains();
		rickList.add(rick);
	}
	


	p2List_item<bodyReturn*>* itemCircle = circleList.getFirst();
	p2List_item<bodyReturn*>* itemRectangle = rectangleList.getFirst();
	p2List_item<bodyReturn*>* itemRick = rickList.getFirst();

	while (itemCircle != NULL )
	{

		App->renderer->Blit(circle, itemCircle->data->getPosition().x - (itemCircle->data->width), itemCircle->data->getPosition().y - (itemCircle->data->height), 0, 1.0f, itemCircle->data->getRotation());
		itemCircle = itemCircle->next;
	}
	while (itemRectangle != NULL)
	{
		App->renderer->Blit(box, itemRectangle->data->getPosition().x - (itemRectangle->data->width*2), itemRectangle->data->getPosition().y -(itemRectangle->data->height / 2), 0, 1.0f, itemRectangle->data->getRotation());
		itemRectangle = itemRectangle->next;
	}
	while (itemRick != NULL)
	{
		App->renderer->Blit(rick, itemRick->data->getPosition().x - (itemRick->data->width * 2), itemRick->data->getPosition().y - (itemRick->data->height / 2), 0, 1.0f, itemRick->data->getRotation());
		itemRick = itemRick->next;
	}

	return UPDATE_CONTINUE;
}