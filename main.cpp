#include <string>

#include "include/Box2D.h"
#include "include/Init.h"
#include "include/Input.h"
#include "include/Physics.h"
#include "include/Sprite.h"
#include "include/Text.h"
#include "include/Tilemap.h"

/**
 * @brief
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char **argv)
{
	// init window and renderer
	GE::Init* game = new GE::Init(640, 480);

	// init box2d world
	game->initBox2DWorld(b2Vec2(0, -10));

	// add box2d listener
	GE::Box2DListener listener;
	game->getBox2DWorld()->SetContactListener(&listener);

	// create sprite for npc
	GE::Sprite* npc = new GE::Sprite(game->getRenderer(), "sprite/npc.png");

	// npc physics
	GE::Box2D* npcPhysics = new GE::Box2D(game->getBox2DWorld(), 100, 300, 32, 32, false, false, "npc");

	// set npc animation clip for spritesheets
	double idle_d[] = {0, 0, 32, 32};
	double idle_r[] = {0, 32, 32, 32};
	double idle_l[] = {0, 32, 32, 32};
	double jump[] = {128, 96, 32, 32};

	// set current clip for npc
	npc->setClip(idle_r);

	// set position of npc on screen
	npc->setPosition(100, 416);

	// size of npc
	double size[2] = {32, 32};

	// create animations for npc
	npc->createAnimation("go_down", {0, 0, 64, 32}, size);
	npc->createAnimation("go_right", {32, 32, 96, 64}, size);
	npc->createAnimation("go_left", {32, 32, 96, 64}, size);
	npc->createAnimation("jumping", {128, 96, 192, 128}, size);

	// keyboard input
	GE::Input* input = new GE::Input();
	
	// other variable
	double dt;
	bool isJumping = false;
	int player_direction = 1;

	// camera scrolling
	game->initCamera(1280, 480);

	// map
	GE::Tilemap* map = new GE::Tilemap(game->getRenderer(), "maps/tes.tmx");
	map->addAllLayer();
	map->addObjectToWorld(game->getBox2DWorld(), "collision", false);
	map->addObjectToWorld(game->getBox2DWorld(), "coin", true);

	// text
	GE::Text* text = new GE::Text(game->getRenderer(), "", "font/agane_roman.ttf", 14, {0, 0, 0, 255}, TTF_STYLE_NORMAL, 640);

	// START SECTION FOR GAME LOOP //

	while (game->update())
	{
		// get delta time
		dt = game->getDeltaTime();

		// START SECTION FOR 2D CAMERA //

		game->setCamera(npc);

		// END SECTION FOR 2D CAMERA //

		// START SECTION FOR PHYSICS //

		game->updateBox2DWorld(dt);
		
		npc->setPosition((double)npcPhysics->getPositionX(), (double)npcPhysics->getPositionY());

		if (isJumping)
		{
			npc->setAnimation("jumping", 0.2); // play jump animation
			bool on_ground = listener.getTouchedBottom() > 0;
			if (on_ground && npcPhysics->getBody()->GetLinearVelocity().y == 0) isJumping = false;
		}

		// END SECTION FOR PHYSICS //

		// START SECTION FOR KEYBOARDS //

		input->updateKeyboard();

		if (input->getKeyboardPressed("Right") && input->getKeyboardPressed("Left"))
		{
			npcPhysics->moveHorizontal(0);
		}
		else if (input->getKeyboardPressed("Right"))
		{
			// play animation of npc
			if (!isJumping)
				npc->setAnimation("go_right", 0.2);

			player_direction = 1;
			npcPhysics->moveHorizontal(300 * dt);
		}
		else if (input->getKeyboardPressed("Left"))
		{
			// play animation of npc
			if (!isJumping)
				npc->setAnimation("go_left", 0.2);

			player_direction = -1;
			npcPhysics->moveHorizontal(-300 * dt);
		}
		
		if (input->getKeyboardPressed("Up") && !isJumping)
		{
			isJumping = true;
			npcPhysics->applyJump(-1.8);
		}

		if (input->getKeyboardReleased("Right"))
		{
			// set animation to idle, and velocity to 0
			if (!isJumping)
				npc->setClip(idle_r);

			npcPhysics->stopMoveHorizontal();
		}
		else if (input->getKeyboardReleased("Left") && !isJumping)
		{
			// set animation to idle, and velocity to 0
			if (!isJumping)
				npc->setClip(idle_l);

			npcPhysics->stopMoveHorizontal();
		}
		else if (input->getKeyboardReleased("Up") && !isJumping)
		{
			if (player_direction) npc->setClip(idle_r);
			else npc->setClip(idle_l);
			npcPhysics->stopMoveHorizontal();
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

	delete(text);
	delete(map);
	delete(npcPhysics);
	delete(npc);
	delete(input);
	delete(game);

	// END SECTION FOR DESTRUCTOR //

	return 0;
}