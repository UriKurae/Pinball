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

	void LoadMapTextures();

public:

	p2List<bodyReturn*> circleList;
	p2List<bodyReturn*> rectangleList;
	p2List<bodyReturn*> rickList;
	
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pinballMap;

	SDL_Rect mapRect;
};
