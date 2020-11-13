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

	int getLifes() { return lifes; };
	void subtractLifes(int lifes);

	void addPoint(int points);
	int getPoints() { return points; };

	int getHighScore() { return highScore; };
	int getLastScore() { return lastScore; };


	void dead();
	void restartGame();

	void setStunTime(float time);
	int getStunTime() { return stunTime; };

	bool canPlay = true;

private:
	int lifes;
	int points;
	int highScore;
	int lastScore;
	float stunTime;
};