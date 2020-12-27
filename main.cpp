#include <iostream>
#include "include/Init.h"
#include "include/Physics.h"
#include "include/Sprite.h"
#include "include/Shape.h"
#include "include/Text.h"
#include "include/Input.h"
#include "include/Tilemap.h"
#include "include/Box2D.h"

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

	// box2d
	b2Vec2 gravity(0.0, 10.0);
	b2World world(gravity);
	GE::Box2D* npcPhysics = new GE::Box2D(&world, 100, 300, 32, 32, false);
	GE::Box2D* ground = new GE::Box2D(&world, 0, 448, 1280, 32, true);
	double block_size[2] = {32, 32};
	GE::Shape* b = new GE::Shape(game->getRenderer());
	GE::Box2D* block = new GE::Box2D(&world, 480, 416, 32, 32, true);

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	float timeStep = 1.f / 60.f;

	// START SECTION FOR GAME LOOP //

	while (game->update())
	{
		// get delta time
		dt = game->getDeltaTime();

		// START SECTION FOR 2D CAMERA //

		game->setCamera(npc);

		// END SECTION FOR 2D CAMERA //

		// START SECTION FOR PHYSICS //

		world.SetGravity(b2Vec2(0, 400 * dt));
		world.Step(timeStep, velocityIterations, positionIterations);
		npc->setPosition((double)npcPhysics->getPositionX(), (double)npcPhysics->getPositionY());

		if (isJumping)
		{
			npc->setAnimation("jumping", 0.2); // play jump animation
			if (npcPhysics->getBody()->GetLinearVelocity().y == 0) isJumping = false;
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
			//play animation of npc
			if (!isJumping)
				npc->setAnimation("go_right", 0.2);

			player_direction = 1;
			npcPhysics->moveHorizontal(300 * dt);
		}
		else if (input->getKeyboardPressed("Left"))
		{
			//play animation of npc
			if (!isJumping)
				npc->setAnimation("go_left", 0.2);

			player_direction = -1;
			npcPhysics->moveHorizontal(-300 * dt);
		}
		
		if (input->getKeyboardPressed("Up") && !isJumping)
		{
			isJumping = true;
			float impulse = npcPhysics->getBody()->GetMass() * -1.2;
			npcPhysics->getBody()->ApplyLinearImpulse(b2Vec2(0, impulse), npcPhysics->getBody()->GetWorldCenter(), true);
		}

		if (input->getKeyboardReleased("Right"))
		{
			//set animation to idle, and velocity to 0
			if (!isJumping)
				npc->setClip(idle_r);

			b2Vec2 vel = npcPhysics->getBody()->GetLinearVelocity();
			vel.x = 0;
			npcPhysics->getBody()->SetLinearVelocity(vel);
		}
		else if (input->getKeyboardReleased("Left") && !isJumping)
		{
			//set animation to idle, and velocity to 0
			if (!isJumping)
				npc->setClip(idle_l);

			b2Vec2 vel = npcPhysics->getBody()->GetLinearVelocity();
			vel.x = 0;
			npcPhysics->getBody()->SetLinearVelocity(vel);
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
		b->drawRectangle(block->getPositionX(), block->getPositionY(), block_size, {0, 0, 0, 255});

		// END SECTION FOR DRAW OBJECTS //

		game->updateCamera();
		game->present(); //update renderer
	}

	// END SECTION FOR GAME LOOP //

	// DESTRUCTOR //

	delete(game);
	delete(npc);
	delete(input);
	delete(map);
	delete(npcPhysics);
	delete(ground);
	delete(b);
	delete(block);

	// END SECTION FOR DESTRUCTOR //

	return 0;
}