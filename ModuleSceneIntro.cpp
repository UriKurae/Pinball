#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "Box2D/Box2D/Box2D.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ballTexture = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ballTexture = App->textures->Load("pinball/Ball.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	pinballMap = App->textures->Load("pinball/Map.png");
	triangleLeft = App->textures->Load("pinball/triangleLeft.png");
	triangleRight = App->textures->Load("pinball/triangleRight.png");
	rightLeverTexture = App->textures->Load("pinball/palancaDerecha.png");
	leftLeverTexture = App->textures->Load("pinball/palancaIzquierda.png");
	canyon = App->textures->Load("pinball/canyon.png");
	bridge = App->textures->Load("pinball/bridge.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");


	//Fonts 
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };	
	yellowFont = App->fonts->Load("pinball/FontY.png", lookupTable, 5);
	// Creation and setup of the ball. 
	if (ball == nullptr) {
		ball = App->physics->createCircle(initialPos.x, initialPos.y, 13.0f, 0);
	}
	ball->bodyTag = "Player";

	 // Creation and setup of the levers
	 CreateLevers();
	 MapChain();
	 mapToDraw = 1;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(ballTexture);
	App->textures->Unload(box);
	App->textures->Unload(rick);
	App->textures->Unload(pinballMap);
	App->textures->Unload(triangleLeft);
	App->textures->Unload(triangleRight);
	App->textures->Unload(rightLeverTexture);
	App->textures->Unload(leftLeverTexture);
	App->textures->Unload(canyon);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (dead == true) {
		if (ball != nullptr && App->player->canPlay) {
		//	App->physics->GetWorld()->DestroyBody(ball->bodyPointer);
			ball->bodyPointer->SetLinearVelocity(b2Vec2(0, 0));
			ball->bodyPointer->SetAngularVelocity(0);
			ball->bodyPointer->SetTransform(b2Vec2(9,14), ball->bodyPointer->GetAngle());
			changeMap = true;
			mapPartToChange4->bodyPointer->SetActive(false);
			App->player->subtractLifes(1);
			//ball = nullptr;
		}
		//ball = App->physics->createCircle(initialPos.x, initialPos.y, 13, 0, "player");
		dead = false;
	}

	App->renderer->Blit(pinballMap, 0, 0, NULL);

	if (changeMap)
	{
		mapToDraw = mapToDraw == 1 ? 2 : 1;
		MapChain();
		changeMap = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN /*&& launched == false*/)
	{
		ball->bodyPointer->ApplyForce(b2Vec2(0, -50), b2Vec2(ball->getPosition().x, ball->getPosition().y), 1);
		launched = true;
		ball->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		leverJointB->SetMotorSpeed(-20.0f);
		if (currentAngleLeverB < 60)
		{
			currentAngleLeverB += 20.0f;
		}
			App->renderer->Blit(rightLeverTexture, 238, 697, NULL, 1.0f, currentAngleLeverB, 48, 10);
	}
	else
	{
		leverJointB->SetMotorSpeed(20.0f);
		if (currentAngleLeverB > -15)
		{
			currentAngleLeverB -= 20.0f;
		}
		App->renderer->Blit(rightLeverTexture, 240, 700, NULL, 1.0f, currentAngleLeverB, 48, 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		leverJointA->SetMotorSpeed(20.0f);
		if (currentAngleLeverA > -60)
		{
			currentAngleLeverA -= 20.0f;
		}
		App->renderer->Blit(leftLeverTexture, 145, 700, NULL, 1.0f, currentAngleLeverA, 11, 10);
		
	}
	else
	{
		leverJointA->SetMotorSpeed(-20.0f);
		if (currentAngleLeverA < 15)
		{
			currentAngleLeverA += 20.0f;
		}
		App->renderer->Blit(leftLeverTexture, 145, 700, NULL, 1.0f, currentAngleLeverA, 11, 10);
	}


	if (App->player->getStunTime() <= 0)
	{
		App->renderer->Blit(ballTexture, (ball->getPosition().x - ball->width), ball->getPosition().y - ball->width, NULL);
		if (mapToDraw == 2)
		{
			App->renderer->Blit(bridge, 0, 0, NULL);
		}
	}
	else
	{
		App->player->setStunTime(App->player->getStunTime() - 5);
		ball->bodyPointer->SetActive(false);
		if (App->player->getStunTime() < 5.0f)
		{
			ball->bodyPointer->SetActive(true);
			switch (whatBumper)
			{
			case 0:
				break;

			case 1:
				//Canyon
				ball->bodyPointer->ApplyForce(b2Vec2(0, -50), b2Vec2(ball->getPosition().x, ball->getPosition().y), 1);
				break;

			case 2:
				//Entrance up-right
				ball->bodyPointer->ApplyForce(b2Vec2(0, 20), b2Vec2(ball->getPosition().x, ball->getPosition().y), 5);
				break;
			}
			
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_REPEAT)
	{
		App->player->dead();
	}

	App->renderer->Blit(triangleLeft, 106, 577, NULL);
	App->renderer->Blit(triangleRight, 300, 577, NULL);
	App->renderer->Blit(canyon, 28, 695, NULL);
	App->renderer->Blit(canyon, 392, 695, NULL);

	App->fonts->BlitText(8, SCREEN_HEIGHT-50, yellowFont, "SCORE:");
	App->fonts->BlitText(8, SCREEN_HEIGHT -23, yellowFont, "LIFES:");
	App->fonts->BlitText(200, SCREEN_HEIGHT - 50, yellowFont, "HIGH SCORE:");
	App->fonts->BlitText(200, SCREEN_HEIGHT - 23, yellowFont, "LAST SCORE:");


	App->fonts->BlitText(110, SCREEN_HEIGHT-50, yellowFont, std::to_string(App->player->getPoints()).c_str());
	App->fonts->BlitText(110, SCREEN_HEIGHT - 23, yellowFont, std::to_string(App->player->getLifes()).c_str());
	App->fonts->BlitText(384, SCREEN_HEIGHT - 50, yellowFont, std::to_string(App->player->getHighScore()).c_str());
	App->fonts->BlitText(384, SCREEN_HEIGHT - 23, yellowFont, std::to_string(App->player->getLastScore()).c_str());

	
	return UPDATE_CONTINUE;
}



void ModuleSceneIntro::MapChain()
{	
	App->physics->createCircle(223, 850, 50, 0, "dead", b2BodyType::b2_staticBody);
		// Map outside boundaries
		// Pivot 0, 0
	int MapBorder[206] = {
		455, 765,
		468, 760,
		472, 740,
		472, 687,
		469, 569,
		472, 413,
		473, 287,
		474, 162,
		472, 119,
		460, 92,
		444, 64,
		416, 39,
		377, 19,
		332, 6,
		285, 0,
		228, 0,
		151, 0,
		114, 10,
		75, 22,
		46, 37,
		20, 64,
		-23, 120,
		-15, 361,
		24, 387,
		39, 416,
		44, 430,
		30, 443,
		31, 496,
		31, 524,
		38, 533,
		26, 550,
		23, 577,
		20, 611,
		19, 640,
		19, 679,
		20, 711,
		20, 743,
		19, 764,
		24, 776,
		37, 780,
		49, 780,
		59, 776,
		64, 755,
		62, 732,
		64, 720,
		84, 730,
		107, 743,
		142, 758,
		171, 772,
		171, 801,
		1, 800,
		0, 562,
		24, 386,
		-4, 295,
		-15, 209,
		-14, 146,
		0, 90,
		20, 41,
		60, 15,
		103, 2,
		140, -8,
		216, -12,
		279, -12,
		328, -6,
		370, 3,
		401, 16,
		432, 34,
		461, 63,
		481, 108,
		483, 152,
		482, 188,
		481, 345,
		481, 507,
		481, 604,
		481, 736,
		480, 764,
		472, 798,
		416, 800,
		345, 800,
		276, 797,
		276, 774,
		313, 754,
		369, 726,
		386, 718,
		385, 772,
		390, 780,
		410, 781,
		426, 776,
		427, 720,
		427, 626,
		422, 573,
		417, 548,
		404, 532,
		428, 474,
		421, 464,
		436, 450,
		443, 444,
		439, 568,
		436, 580,
		435, 621,
		435, 705,
		437, 755,
		445, 764
	};

	// Pivot 0, 0
	int MapInterior[216] = {
		7, 535,
		7, 800,
		170, 800,
		170, 771,
		59, 715,
		59, 770,
		47, 778,
		30, 775,
		22, 767,
		25, 575,
		30, 550,
		46, 534,
		34, 517,
		35, 444,
		50, 433,
		27, 381,
		4, 324,
		32, 306,
		64, 381,
		69, 375,
		34, 271,
		20, 222,
		22, 160,
		37, 121,
		60, 97,
		96, 78,
		140, 70,
		182, 72,
		207, 79,
		223, 87,
		225, 82,
		225, 61,
		232, 51,
		243, 49,
		256, 54,
		257, 68,
		258, 142,
		264, 162,
		267, 183,
		265, 201,
		262, 213,
		260, 220,
		265, 224,
		272, 221,
		278, 217,
		290, 217,
		299, 221,
		305, 228,
		306, 239,
		316, 239,
		317, 230,
		310, 221,
		299, 205,
		290, 178,
		286, 156,
		287, 127,
		292, 104,
		302, 84,
		318, 71,
		342, 60,
		376, 55,
		407, 61,
		436, 74,
		457, 99,
		470, 130,
		471, 169,
		459, 216,
		440, 265,
		408, 329,
		391, 376,
		400, 383,
		410, 354,
		421, 324,
		456, 322,
		433, 390,
		436, 395,
		448, 386,
		465, 390,
		470, 416,
		468, 429,
		438, 442,
		420, 465,
		424, 477,
		400, 535,
		416, 552,
		421, 580,
		423, 617,
		424, 772,
		414, 778,
		397, 778,
		387, 773,
		387, 713,
		276, 771,
		275, 798,
		458, 795,
		461, 493,
		474, 410,
		467, 326,
		478, 233,
		492, 137,
		456, 52,
		320, 17,
		152, 16,
		57, 28,
		14, 88,
		5, 192,
		-8, 312,
		11, 423
	};

	// Pivot 0, 0
	int wallLeverLeft[38] = {
		134, 709,
		113, 700,
		87, 685,
		69, 674,
		60, 666,
		60, 624,
		59, 603,
		60, 595,
		66, 590,
		67, 602,
		66, 616,
		66, 636,
		68, 648,
		74, 660,
		92, 672,
		108, 680,
		126, 688,
		138, 694,
		135, 704
	};

	// Pivot 0, 0
	int WallLeverRight[20] = {
		301, 699,
		323, 687,
		346, 676,
		372, 660,
		378, 643,
		379, 590,
		386, 596,
		386, 665,
		379, 675,
		306, 711
	};

	// Pivot 0, 0
	int wallTriangleLeft[38] = {
		140, 659,
		131, 654,
		119, 648,
		109, 643,
		106, 640,
		106, 618,
		106, 598,
		106, 583,
		106, 579,
		106, 575,
		110, 577,
		116, 587,
		123, 601,
		132, 619,
		140, 638,
		145, 648,
		148, 655,
		148, 660,
		144, 662
	};
	
	// Pivot 0, 0
	int wallTriangleRight[42] = {
		300, 661,
		298, 657,
		305, 640,
		315, 620,
		321, 608,
		327, 596,
		332, 584,
		336, 578,
		340, 578,
		341, 581,
		341, 588,
		341, 598,
		341, 611,
		341, 621,
		341, 632,
		341, 638,
		340, 643,
		335, 647,
		322, 654,
		312, 659,
		304, 663
	};

	// Pivot 0, 0
	int InteriorWallBridge[208] = {
	401, 376,
	411, 354,
	420, 325,
	427, 296,
	434, 258,
	439, 224,
	441, 191,
	441, 150,
	432, 118,
	418, 92,
	398, 70,
	380, 57,
	362, 53,
	367, 67,
	373, 83,
	378, 101,
	381, 109,
	407, 122,
	412, 148,
	401, 138,
	392, 127,
	368, 125,
	353, 132,
	341, 133,
	350, 114,
	347, 96,
	336, 76,
	314, 49,
	278, 33,
	222, 33,
	184, 32,
	157, 34,
	127, 40,
	99, 48,
	73, 61,
	49, 82,
	30, 112,
	21, 143,
	19, 164,
	19, 192,
	21, 220,
	23, 248,
	28, 274,
	32, 298,
	41, 329,
	51, 357,
	60, 378,
	67, 378,
	48, 333,
	35, 285,
	30, 241,
	25, 193,
	24, 162,
	28, 135,
	41, 105,
	59, 81,
	79, 65,
	105, 53,
	140, 44,
	176, 38,
	212, 37,
	259, 38,
	290, 43,
	313, 55,
	324, 68,
	335, 85,
	342, 104,
	340, 122,
	332, 133,
	335, 160,
	348, 175,
	348, 147,
	365, 135,
	389, 135,
	403, 157,
	392, 185,
	416, 171,
	422, 141,
	411, 114,
	384, 102,
	368, 58,
	395, 75,
	410, 90,
	425, 118,
	434, 147,
	435, 190,
	433, 227,
	430, 253,
	424, 287,
	416, 317,
	407, 346,
	396, 367,
	394, 377,
	394, 377,
	394, 377,
	433, 227,
	430, 253,
	424, 287,
	416, 317,
	407, 346,
	396, 367,
	394, 377,
	394, 377,
	394, 377
	};

	// Pivot 0, 0
	int Wall1[34] = {
		61, 179,
		57, 172,
		63, 154,
		72, 140,
		84, 128,
		94, 119,
		108, 111,
		128, 108,
		134, 113,
		128, 116,
		118, 117,
		103, 123,
		90, 133,
		79, 143,
		71, 157,
		66, 168,
		66, 177
	};

	int Wall2[32] = {
	162, 110,
	172, 107,
	183, 110,
	198, 115,
	212, 124,
	221, 135,
	229, 151,
	232, 168,
	229, 177,
	224, 168,
	221, 153,
	217, 140,
	207, 128,
	196, 122,
	179, 117,
	164, 115
	};

	int MapBorderRight[22] = {
	434, 456,
	438, 441,
	466, 424,
	468, 403,
	457, 386,
	438, 382,
	453, 335,
	462, 300,
	467, 268,
	497, 268,
	510, 415
	};

	// Pivot 0, 0
	int InteriorCircle[32] = {
		394, 102,
		414, 117,
		420, 139,
		391, 184,
		415, 172,
		426, 144,
		420, 116,
		396, 96,
		366, 92,
		346, 104,
		331, 136,
		335, 160,
		348, 176,
		336, 140,
		350, 107,
		368, 98
	};
	
	if (mapToDraw == 0)
	{

		mapPartToChange1 = App->physics->createChain(0, 0, MapBorder, 206);
		mapPartToChange2 = App->physics->createChain(0, 0, InteriorWallBridge, 186);
		mapPartToChange3 = App->physics->createChain(0, 0, MapInterior, 216);
		mapPartToChange4 = App->physics->createChain(0, 0, MapBorderRight, 22);
		mapPartToChange5 = App->physics->createChain(0, 0, InteriorCircle, 32);

		mapPartToChange3->bodyPointer->SetActive(false);
		mapPartToChange4->bodyPointer->SetActive(false);
		mapPartToChange5->bodyPointer->SetActive(false);

		//Little Bumpers
		App->physics->createCircle(198, 189, 13.0f, 1, "SmallBumper", b2BodyType::b2_staticBody);
		App->physics->createCircle(285, 242, 13.0f, 1, "SmallBumper", b2BodyType::b2_staticBody);
		App->physics->createCircle(439, 416, 2.0f, 1, "lateralBumper", b2BodyType::b2_staticBody);
		App->physics->createCircle(320, 90, 7.0f, 1, "rightPassage", b2BodyType::b2_staticBody);
		App->physics->createCircle(243, 65, 13.0f, 1, "upperBumper", b2BodyType::b2_staticBody);

		//Little bumpers that hold the ball for some seconds
		App->physics->createCircle(405, 715, 13.0f, 0, "canyon", b2BodyType::b2_staticBody);
		App->physics->createCircle(41, 715, 13.0f, 0, "canyon", b2BodyType::b2_staticBody);
		App->physics->createCircle(375, 154, 7.0f, 1, "entryBumper", b2BodyType::b2_staticBody);

		//Big centric bumper
		App->physics->createCircle(224, 355, 30, 1, "BigBumper", b2BodyType::b2_staticBody);

		App->physics->createChain(0, 0, wallLeverLeft, 38);
		App->physics->createChain(0, 0, WallLeverRight, 20);
		App->physics->createChain(0, 0, wallTriangleLeft, 38);
		App->physics->createChain(0, 0, wallTriangleRight, 42);
		App->physics->createChain(0, 0, Wall1, 34);
		App->physics->createChain(0, 0, Wall2, 32);

		//Sensors
		App->physics->createCircle(25, 340, 7.0f, 2, "sensor", b2BodyType::b2_staticBody);
		App->physics->createCircle(425, 340, 7.0f, 2, "sensor", b2BodyType::b2_staticBody);
		App->physics->createCircle(370, 117, 4.0f, 2, "sensor", b2BodyType::b2_staticBody);
	}
	if (mapToDraw == 1)
	{	
		mapPartToChange3->bodyPointer->SetActive(false);
		mapPartToChange5->bodyPointer->SetActive(false);
		mapPartToChange1->bodyPointer->SetActive(true);
		mapPartToChange2->bodyPointer->SetActive(true);
	}
	if (mapToDraw == 2)
	{
		mapPartToChange1->bodyPointer->SetActive(false);
		mapPartToChange2->bodyPointer->SetActive(false);
		mapPartToChange3->bodyPointer->SetActive(true);
		mapPartToChange4->bodyPointer->SetActive(true);
		mapPartToChange5->bodyPointer->SetActive(true);
	}
}

void ModuleSceneIntro::CreateLevers()
{
	if (anchorPointA == nullptr) {
		anchorPointA = App->physics->createRectangle(150, 710, 0.1f, 0.1f, 0);
		leverA = App->physics->createRectangle(174, 710, 0.65f, 0.1f, 0, b2BodyType::b2_dynamicBody);



		anchorPointB = App->physics->createRectangle(294, 710, 0.1f, 0.1f, 0);
		leverB = App->physics->createRectangle(270, 710, 0.65f, 0.1f, 0, b2BodyType::b2_dynamicBody);


		b2RevoluteJointDef revoluteJointDef;
		revoluteJointDef.Initialize(leverA->bodyPointer, anchorPointA->bodyPointer, anchorPointA->bodyPointer->GetWorldCenter());
		revoluteJointDef.collideConnected = false;
		revoluteJointDef.lowerAngle = DEGTORAD * -20;
		revoluteJointDef.upperAngle = DEGTORAD * 45;
		revoluteJointDef.enableLimit = true;
		revoluteJointDef.maxMotorTorque = 50.0f;
		revoluteJointDef.motorSpeed = 0.0f;
		revoluteJointDef.enableMotor = true;
		leverJointA = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&revoluteJointDef);

		revoluteJointDef.Initialize(leverB->bodyPointer, anchorPointB->bodyPointer, anchorPointB->bodyPointer->GetWorldCenter());
		revoluteJointDef.collideConnected = false;
		revoluteJointDef.lowerAngle = DEGTORAD * -45;
		revoluteJointDef.upperAngle = DEGTORAD * 20;
		revoluteJointDef.enableLimit = true;
		revoluteJointDef.maxMotorTorque = 50.0f;
		revoluteJointDef.motorSpeed = 0.0f;
		revoluteJointDef.enableMotor = true;
		leverJointB = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&revoluteJointDef);
	}
	
}


void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	App->audio->PlayFx(bonus_fx);

}

void ModuleSceneIntro::restartGame()
{

	dead = true;
}
