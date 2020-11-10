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

	void CreateLevers();

public:

	SDL_Texture* ballTexture;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pinballMap;
	
	SDL_Texture* triangleLeft;
	SDL_Texture* triangleRight;


	// Player's ball
	PhysBody* ball;
	bool launched = false;

	// Player's levers
	b2RevoluteJoint* leverJointA;
	PhysBody* anchorPointA;
	PhysBody* leverA;

	b2RevoluteJoint* leverJointB;
	PhysBody* anchorPointB;
	PhysBody* leverB;
};
