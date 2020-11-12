#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void addPoint(int points);
	int getPoints() { return points; };

	void setStunTime(float time);
	int getStunTime() { return stunTime; };


private:
	int points;
	float stunTime;
};