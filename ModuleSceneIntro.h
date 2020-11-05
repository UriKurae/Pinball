#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
#include "ModulePhysics.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void MapChain();

public:

	SDL_Texture* ballTexture;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pinballMap;

	// Player's ball
	PhysBody* ball;

};
