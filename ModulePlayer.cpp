#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "ModuleDeadScene.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	points = 0;
	lifes = 3;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::subtractLifes(int lifes)
{
	this->lifes -= lifes;
	if (this->lifes <= 0) {
		App->dead->Enable();
		App->scene_intro->Disable();
	}
}

void ModulePlayer::addPoint(int points)
{
	this->points += points;
}

void ModulePlayer::setStunTime(float time)
{
	this->stunTime = time;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}