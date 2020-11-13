#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "ModulePhysics.h"

class PhysBody;
class SDL_Texture;

class ModuleDeadScene : public Module
{
public:
	ModuleDeadScene(Application* app, bool start_enabled = true);
	~ModuleDeadScene();

	void playSound();
	bool Start();
	update_status Update();
	bool CleanUp();


public:


	//Fonts
	int yellowFont = -1;
	int yellowFont2 = -1;

	uint GameOver;

};
