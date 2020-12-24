#include <iostream>
#include "include/Init.h"
#include "include/Physics.h"
#include "include/Sprite.h"
#include "include/Shape.h"
#include "include/Text.h"

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
	npc->setPosition(320, 416);

	//create physics for npc (false = dynamic physics)
	GE::Physics* npcPhysics = new GE::Physics(false);

	//set npc physics body to npc sprites
	npcPhysics->setBody(npc);

	//set npc physics collision rect
	npcPhysics->setCollisionRect(npc->getRect());

	//set the gravity for npc physics
	npcPhysics->setGravity(200);

	// size of npc
	double size[2] = {32, 32};

	//create animations for npc
	npc->createAnimation("go_down", {0, 0, 64, 32}, size);
	npc->createAnimation("go_right", {32, 32, 96, 64}, size);
	npc->createAnimation("go_left", {32, 32, 96, 64}, size);
	npc->createAnimation("jumping", {128, 96, 192, 128}, size);

	//create ground
	GE::Shape* ground = new GE::Shape(game->getRenderer());

	//ground physics
	GE::Physics* groundPhysics = new GE::Physics(true);
	groundPhysics->setBody(ground);
	groundPhysics->setCollisionRect(ground->getRect());

	//ground size
	double ground_size[2] = {1280, 32};

	//create block
	GE::Shape* block = new GE::Shape(game->getRenderer());

	//block physics
	GE::Physics* blockPhysics = new GE::Physics(true);
	blockPhysics->setBody(block);
	blockPhysics->setCollisionRect(block->getRect());

	//block size
	double block_size[2] = {64, 32};

	//other variable
	const Uint8 *key = game->keyListener(); //set keyboard listener
	double dt;
	std::string player_state = "go_right";
	std::string collide = "";
	bool isJumping = false;
	bool isCeiling = false;
	double timer = 0;

	// camera scrolling
	game->initCamera(1280, 480);

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

		groundPhysics->setCollisionRect(ground->getRect());
		blockPhysics->setCollisionRect(block->getRect());

		if (npcPhysics->detectAABB(groundPhysics) == "topWall") {
			isJumping = false;
		}

		collide = npcPhysics->detectAABB(blockPhysics);

		// if npc collide with top of the shape
		if (collide == "topWall")
		{
			isJumping = false;
		}
		else if (collide == "bottomWall") // if npc collide with bottom of the shape
		{
			isJumping = true;
		}
		else if (collide == "fallFromTopWall") // if npc not collide with anything (fall from shape)
		{
			isJumping = true;
		}

		// END SECTION FOR PHYSICS //

		// START SECTION FOR KEYBOARDS //

		if (key[SDL_SCANCODE_RIGHT] && key[SDL_SCANCODE_LEFT]) // if keyboard right and left pressed at the same time
		{
			npcPhysics->setVelocityX(0); // stop npc
		}
		else if (key[SDL_SCANCODE_RIGHT]) // if keyboard right pressed
		{
			player_state = "go_right"; // set player state to go_right
			
			//play animation of npc
			if (!isJumping)
				npc->setAnimation(player_state, 0.2);

			//set npc horizontal velocity
			npcPhysics->setVelocityX(200);
		}
		else if (key[SDL_SCANCODE_LEFT])
		{
			player_state = "go_left"; // set player state to go_left

			//play animation of npc
			if (!isJumping)
				npc->setAnimation(player_state, 0.2);

			//set npc horizontal velocity
			npcPhysics->setVelocityX(200 * -1);
		}
		else //key releases
		{
			if (player_state == "go_right")
			{
				//set animation to idle, and velocity to 0
				npc->setClip(idle_r);
				npcPhysics->setVelocityX(0);
			}
			if (player_state == "go_left")
			{
				//set animation to idle, and velocity to 0
				npc->setClip(idle_l);
				npcPhysics->setVelocityX(0);
			}
			if (isJumping) // if npc is jumping
			{
				//play jump animation
				npc->setAnimation("jumping", 0.2);
			}
		}

		// if keyboard up is pressed and npc is not jumping
		if (key[SDL_SCANCODE_UP] && !isJumping)
		{
			//set jumping condition and vertical velocity of npc
			isJumping = true;
			npcPhysics->setVelocityY(-npcPhysics->getGravity());
		}

		// END SECTION FOR KEYBOARDS //

		// START SECTION FOR DRAW OBJECTS //

		npc->draw(dt); //draw npc
		ground->drawRectangle(0, 448, ground_size, {0, 0, 0, 255}); //draw ground
		block->drawRectangle(600, 384, block_size, {0, 0, 0, 255}); //draw block

		// END SECTION FOR DRAW OBJECTS //

		game->updateCamera();
		game->present(); //update renderer
	}

	// END SECTION FOR GAME LOOP //

	return 0;
}