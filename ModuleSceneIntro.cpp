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
	palancaDerecha = App->textures->Load("pinball/palancaDerecha.png");
	palancaIzquierda = App->textures->Load("pinball/palancaIzquierda.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");


	//Fonts 
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz  0123456789.,ªº?!*$%&()+-/:;<=>@·    " };	
	yellowFont = App->fonts->Load("pinball/FontY.png", lookupTable, 5);
	// Creation and setup of the ball. 

	 ball = App->physics->createCircle(454.0f, 731.0f, 13.0f);
	 ball->bodyTag = "Player";
	 // Creation and setup of the levers
	 CreateLevers();
	 MapChain();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(pinballMap, 0, 0, NULL);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && launched == false)
	{
		ball->bodyPointer->ApplyForce(b2Vec2(0, -50), b2Vec2(ball->getPosition().x, ball->getPosition().y), 1);
		launched = true;
		ball->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		leverJointB->SetMotorSpeed(-10.0f);
		App->renderer->Blit(palancaDerecha, 238, 697, NULL, 1.0f, (0, 60), 48, 10);
	}
	else
	{
		leverJointB->SetMotorSpeed(10.0f);
		App->renderer->Blit(palancaDerecha, 240, 700, NULL, 1.0f, (0, -15), 48, 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		leverJointA->SetMotorSpeed(10.0f);
		App->renderer->Blit(palancaIzquierda, 145, 700, NULL, 1.0f, (0, -60), 11, 10);
	}
	else
	{
		leverJointA->SetMotorSpeed(-10.0f);
		App->renderer->Blit(palancaIzquierda, 145, 700, NULL, 1.0f, (0, 15), 11, 10);
	}


	App->renderer->Blit(ballTexture, (ball->getPosition().x - ball->width), ball->getPosition().y - ball->width, NULL);
	App->renderer->Blit(triangleLeft, 106, 577, NULL);
	App->renderer->Blit(triangleRight, 300, 577, NULL);

	App->fonts->BlitText(8, 10, yellowFont, "SCORE:");

	App->fonts->BlitText(200, 10, yellowFont, std::to_string(App->player->getPoints()).c_str());

	return UPDATE_CONTINUE;
}



void ModuleSceneIntro::MapChain()
{
	//Little Bumpers
	PhysBody* bumper1 = App->physics->createCircle(198, 189, 13, "SmallBumper",b2BodyType::b2_staticBody);
	PhysBody* bumper2 = App->physics->createCircle(285, 242, 13, "SmallBumper",b2BodyType::b2_staticBody);


	//Big centric bumper
	App->physics->createCircle(224, 355, 50, "BigBumper", b2BodyType::b2_staticBody);

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

	App->physics->createChain(0, 0, MapBorder, 206);

	// Pivot 0, 0
	/*int MapInterior[214] = {
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
		30, 381,
		13, 350,
		43, 331,
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
		473, 203,
		474, 89,
		431, 24,
		314, 8,
		80, 17,
		10, 85,
		6, 182,
		6, 298,
		3, 402
	};

	App->physics->createChain(0, 0, MapInterior, 214);*/

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

	App->physics->createChain(0, 0, wallLeverLeft, 38);



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

	App->physics->createChain(0, 0, WallLeverRight, 20);

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

	App->physics->createChain(0, 0, wallTriangleLeft, 38);
	
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

	App->physics->createChain(0, 0, wallTriangleRight, 42);

	// Pivot 0, 0
	int InteriorWallBridge[186] = {
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
		405, 166,
		392, 174,
		352, 165,
		344, 152,
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
		333, 145,
		336, 163,
		345, 169,
		360, 176,
		374, 177,
		392, 181,
		405, 178,
		414, 168,
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
		394, 377
	};

	App->physics->createChain(0, 0, InteriorWallBridge, 186);

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

	App->physics->createChain(0, 0, Wall1, 34);

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

	App->physics->createChain(0, 0, Wall2, 32);
}

void ModuleSceneIntro::CreateLevers()
{
	anchorPointA = App->physics->createRectangle(150, 710, 0.1f, 0.1f );
	leverA = App->physics->createRectangle(174, 710, 0.65f, 0.1f, b2BodyType::b2_dynamicBody);
	
	

	anchorPointB = App->physics->createRectangle(294, 710, 0.1f, 0.1f);
	leverB = App->physics->createRectangle(270, 710, 0.65f, 0.1f, b2BodyType::b2_dynamicBody);


	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.Initialize(leverA->bodyPointer, anchorPointA->bodyPointer, anchorPointA->bodyPointer->GetWorldCenter());
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.lowerAngle = DEGTORAD * -20;
	revoluteJointDef.upperAngle = DEGTORAD * 45;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.maxMotorTorque = 50.0f;
	revoluteJointDef.motorSpeed = 0.0f;
	revoluteJointDef.enableMotor = true;
	leverJointA =(b2RevoluteJoint*) App->physics->GetWorld()->CreateJoint(&revoluteJointDef);

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


void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	App->audio->PlayFx(bonus_fx);

}