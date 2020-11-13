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


bool ModuleDeadScene::Start()
{
	bool ret = true;

	//Fonts 
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };
	yellowFont = App->fonts->Load("pinball/FontY.png", lookupTable, 5);

	return ret;
}

update_status ModuleDeadScene::Update()
{
	
	App->fonts->BlitText(8, 10, yellowFont, "GAME OVER");


	return UPDATE_CONTINUE;
}



bool ModuleDeadScene::CleanUp() {

	bool ret = true;

	

	return ret;
}