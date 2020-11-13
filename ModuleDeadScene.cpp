#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleDeadScene.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"


ModuleDeadScene::ModuleDeadScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleDeadScene::~ModuleDeadScene()
{

}


void ModuleDeadScene::playSound()
{
	App->audio->PlayFx(GameOver);

}

bool ModuleDeadScene::Start()
{
	bool ret = true;

	//Fonts 
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	yellowFont = App->fonts->Load("pinball/FontYred.png", lookupTable, 5);
	yellowFont2 = App->fonts->Load("pinball/FontY.png", lookupTable, 5);
	GameOver = App->audio->LoadFx("pinball/GameOver.wav");

	return ret;
}

update_status ModuleDeadScene::Update()
{
	
	App->fonts->BlitText((SCREEN_WIDTH/2)-140, (SCREEN_HEIGHT/2)-88, yellowFont, "GAME OVER");
	App->fonts->BlitText((SCREEN_WIDTH / 2) - 80, (SCREEN_HEIGHT / 2), yellowFont2, "SCORE:");
	App->fonts->BlitText((SCREEN_WIDTH / 2) + 30, (SCREEN_HEIGHT / 2) , yellowFont2, std::to_string(App->player->getPoints()).c_str());
	App->fonts->BlitText((SCREEN_WIDTH / 2) - 161, (SCREEN_HEIGHT / 2) +31, yellowFont2, "HIGH SCORE:");
	App->fonts->BlitText((SCREEN_WIDTH / 2) + 30, (SCREEN_HEIGHT / 2) + 31, yellowFont2, std::to_string(App->player->getHighScore()).c_str());
	App->fonts->BlitText((SCREEN_WIDTH / 2) -190, (SCREEN_HEIGHT / 2)+175, yellowFont2, "PRESS <SPACE> TO RESTART");




	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		App->player->restartGame();

	}

	return UPDATE_CONTINUE;
}



bool ModuleDeadScene::CleanUp() {

	bool ret = true;

	

	return ret;
}