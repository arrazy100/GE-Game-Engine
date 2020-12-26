#include <iostream>
#include "include/Init.h"
#include "include/Physics.h"
#include "include/Sprite.h"
#include "include/Shape.h"
#include "include/Text.h"
#include "include/Input.h"
#include "include/Tilemap.h"

/**
 * @brief
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char **argv)
{
	//init window and renderer
	GE::Init* game = new GE::Init(640, 480);

	//create sprite for npc
	GE::Sprite* npc = new GE::Sprite(game->getRenderer(), "sprite/npc.png");

	//set npc animation clip for spritesheets
	double idle_d[] = {0, 0, 32, 32};
	double idle_r[] = {0, 32, 32, 32};
	double idle_l[] = {0, 32, 32, 32};
	double jump[] = {128, 96, 32, 32};

	//set current clip for npc
	npc->setClip(idle_r);

	//set position of npc on screen
	npc->setPosition(100, 416);

	//create physics for npc (false = dynamic physics)
	GE::Physics* npcPhysics = new GE::Physics(false);

	//set npc physics body to npc sprites
	npcPhysics->setBody(npc);
	npcPhysics->update();

	//set the gravity for npc physics
	npcPhysics->setGravity(200);

	// size of npc
	double size[2] = {32, 32};

	//create animations for npc
	npc->createAnimation("go_down", {0, 0, 64, 32}, size);
	npc->createAnimation("go_right", {32, 32, 96, 64}, size);
	npc->createAnimation("go_left", {32, 32, 96, 64}, size);
	npc->createAnimation("jumping", {128, 96, 192, 128}, size);

	//keyboard input
	GE::Input* input = new GE::Input();
	
	//other variable
	double dt;
	std::string collide = "";
	bool isJumping = false;
	bool isCeiling = false;
	int player_direction = 1;

	// camera scrolling
	game->initCamera(1280, 480);

	// map
	GE::Tilemap* map = new GE::Tilemap(game->getRenderer(), "maps/tes.tmx");
	map->addAllLayer();
	std::vector<GE::Physics*> mapPhysics = map->getObjectPhysics();

	// START SECTION FOR GAME LOOP //

	while (game->update())
	{
		// get delta time
		dt = game->getDeltaTime();

		// START SECTION FOR 2D CAMERA //

		game->setCamera(npc);

		// END SECTION FOR 2D CAMERA //

		// START SECTION FOR PHYSICS //

		// npc jump condition
		if (isJumping)
		{
			npc->setAnimation("jumping", 0.2); // play jump animation
			npcPhysics->setVelocityY(npcPhysics->getVelocityY() + (npcPhysics->getGravity() * 1.5) * dt); // adding y velocity per seconds

			// limit y velocity of npc
			if (npcPhysics->getVelocityY() > npcPhysics->getGravity())
				npcPhysics->setVelocityY(npcPhysics->getGravity()); // limit y velocity to physics gravity
		}

		for (int i = 0; i < mapPhysics.size(); i++)
		{
			collide = npcPhysics->detectAABB(mapPhysics[i]);
			if (collide == "top")
			{
				isJumping = false;
			}
			else if (collide == "bottom") // if npc collide with bottom of the shape
			{
				isJumping = true;
			}
			else if (collide == "fallFromTop") // if npc not collide with anything (fall from shape)
			{
				isJumping = true;
			}
			else if (collide == "right")
			{
				npcPhysics->setVelocityX(0);
				npcPhysics->setRect("right", mapPhysics[i]->getRect("left") - 1);
			}
			else if (collide == "left")
			{
				npcPhysics->setVelocityX(0);
				npcPhysics->setRect("left", mapPhysics[i]->getRect("right") + 1);
			}
		}
		// END SECTION FOR PHYSICS //

		// START SECTION FOR KEYBOARDS //

		input->updateKeyboard();

		if (input->getKeyboardPressed("Right") && input->getKeyboardPressed("Left"))
		{
			npcPhysics->setVelocityX(0);
		}
		else if (input->getKeyboardPressed("Right"))
		{
			//play animation of npc
			if (!isJumping)
				npc->setAnimation("go_right", 0.2);

			player_direction = 1;
			//set npc horizontal velocity
			npcPhysics->setVelocityX(200);
		}
		else if (input->getKeyboardPressed("Left"))
		{
			//play animation of npc
			if (!isJumping)
				npc->setAnimation("go_left", 0.2);

			player_direction = -1;
			//set npc horizontal velocity
			npcPhysics->setVelocityX(200 * -1);
		}
		
		if (input->getKeyboardPressed("Up") && !isJumping)
		{
			isJumping = true;
			npcPhysics->setVelocityY(-npcPhysics->getGravity());
		}

		if (input->getKeyboardReleased("Right") && !isJumping)
		{
			//set animation to idle, and velocity to 0
			npc->setClip(idle_r);
			npcPhysics->setVelocityX(0);
		}
		else if (input->getKeyboardReleased("Left") && !isJumping)
		{
			//set animation to idle, and velocity to 0
			npc->setClip(idle_l);
			npcPhysics->setVelocityX(0);
		}
		else if (input->getKeyboardReleased("Up") && !isJumping)
		{
			if (player_direction) npc->setClip(idle_r);
			else npc->setClip(idle_l);
		}

		// END SECTION FOR KEYBOARDS //

		// START SECTION FOR DRAW OBJECTS //

		map->render(dt);
		npc->draw(dt); //draw npc

		// END SECTION FOR DRAW OBJECTS //

		game->updateCamera();
		game->present(); //update renderer
	}

	// END SECTION FOR GAME LOOP //

	// DESTRUCTOR //

	delete(game);
	delete(npc);
	delete(npcPhysics);
	delete(input);
	delete(map);

	// END SECTION FOR DESTRUCTOR //

	return 0;
}