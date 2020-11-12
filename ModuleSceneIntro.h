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

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	SDL_Texture* ballTexture;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pinballMap;
	SDL_Texture* rightLeverTexture;
	SDL_Texture* leftLeverTexture;

	SDL_Texture* triangleLeft;
	SDL_Texture* triangleRight;


	// Player's ball
	PhysBody* ball;
	bool launched = false;

	// Player's levers
	b2RevoluteJoint* leverJointA;
	PhysBody* anchorPointA;
	PhysBody* leverA;
	float currentAngleLeverA = 15.0f;

	b2RevoluteJoint* leverJointB;
	PhysBody* anchorPointB;
	PhysBody* leverB;
	float currentAngleLeverB = -15.0f;

	//Fonts
	int yellowFont = -1;
	uint bonus_fx;
};
