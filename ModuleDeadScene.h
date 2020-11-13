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

	bool Start();
	update_status Update();
	bool CleanUp();


public:


	//Fonts
	int yellowFont = -1;

};
