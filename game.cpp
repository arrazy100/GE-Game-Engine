#include <Box2D.h>
#include <Init.h>
#include <Input.h>
#include <Physics.h>
#include <Sprite.h>
#include <Sound.h>
#include <Text.h>
#include <Tilemap.h>

#include "StateMachine.h"

class game : public StateMachine
{
    private:
        GE::Init* _init = NULL;

        double dt = 0;
        int coin = 0;
        bool isJumping = false;
        int player_direction = 1;

        std::unique_ptr<GE::Sound> coin_collect;
        std::unique_ptr<GE::Sound> bg;

        std::unique_ptr<GE::Input> input;

        std::unique_ptr<GE::Sprite> npc;

        std::unique_ptr<GE::Box2D> npcPhysics;

        std::unique_ptr<GE::Tilemap> map;

        std::unique_ptr<GE::Text> text;

        GE::Box2DListener listener;

        // size of npc
        double size[2];

        // set npc animation clip for spritesheets
        double idle_d[4];
        double idle_r[4];
        double idle_l[4];
        double jump[4];

    public:
        std::string identity() override
        {
            return "Game";
        }

        void init(GE::Init* init) override
        {
            _init = init;

            // coin sound effect
            coin_collect = std::make_unique<GE::Sound>();
            coin_collect->loadSoundEffect("sound/coin_collect.wav");
            coin_collect->changeVolume(50);

            // bg
            bg = std::make_unique<GE::Sound>();
            bg->loadMusic("sound/bg.mp3");
            bg->changeVolume(100);
            bg->play(-1);

            // keyboard input
            input = std::make_unique<GE::Input>();

            // init box2d world
            _init->initBox2DWorld(b2Vec2(0, 2));

            // add box2d listener
            _init->getBox2DWorld()->SetContactListener(&listener);

            // size of npc
            size[0] = 32;
            size[1] = 32;

            // set npc animation clip for spritesheets
            idle_d[0] = 0;
            idle_d[1] = 0;
            idle_d[2] = 32;
            idle_d[3] = 32;

            idle_r[0] = 0;
            idle_r[1] = 32;
            idle_r[2] = 32;
            idle_r[3] = 32;

            idle_l[0] = 0;
            idle_l[1] = 32;
            idle_l[2] = 32;
            idle_l[3] = 32;

            jump[0] = 128;
            jump[1] = 96;
            jump[2] = 32;
            jump[3] = 32;

            // create sprite for npc
            npc = std::make_unique<GE::Sprite>(_init->getRenderer(), "sprite/npc.png");

            // npc physics
            npcPhysics = std::make_unique<GE::Box2D>(_init->getBox2DWorld(), 100, 300, 32, 32, false, false, "npc");
            npcPhysics->addFixture(8, 8, b2Vec2(0, 32), true, "bottom");
            npcPhysics->addFixture(8, 8, b2Vec2(0, 0), true, "top");
            npcPhysics->addFixture(8, 8, b2Vec2(16, 16), true, "right");
            npcPhysics->addFixture(8, 8, b2Vec2(-16, 16), true, "left");

            // set current clip for npc
            npc->setClip(idle_r);

            // set position of npc on screen
            npc->setPosition(100, 416);

            // create animations for npc
            npc->createAnimation("go_down", {0, 0, 64, 32}, size);
            npc->createAnimation("go_right", {32, 32, 96, 64}, size);
            npc->createAnimation("go_left", {32, 32, 96, 64}, size);
            npc->createAnimation("jumping", {128, 96, 192, 128}, size);

            // camera scrolling
            _init->initCamera(1280, 480);

            // map
            map = std::make_unique<GE::Tilemap>(_init->getRenderer(), "maps/tes.tmx");
            map->addNormalLayer("main");
            map->addPhysicsFromObject(_init->getBox2DWorld(), "collision");
            map->addRemovableObjectToWorld(_init->getBox2DWorld(), "coin", true, true);

            // text
            SDL_Color c = {0, 0, 0, 255};
            text = std::make_unique<GE::Text>(_init->getRenderer(), "", "font/agane_roman.ttf", 14, c, TTF_STYLE_NORMAL, 640);
        }

        void update() override
        {
            dt = _init->getDeltaTime();

            _init->setCamera(npc.get());

            _init->updateBox2DWorld(dt);

            if (npcPhysics->touchWithBody("coin"))
            {
                for (auto it = map->getRemovableObjects().begin(); it != map->getRemovableObjects().end();)
                {
                    if (it->second->getBody() == npcPhysics->touchWithBody("coin"))
                    {
                        coin_collect->play(0);
                        coin++;
                        map->getRemovableObjects().erase(it);
                    }
                    else ++it;
                }
            }

            /*
            if (npcPhysics->touchWithFixture("enemy", "top"))
            {

            }
            */
            
            npc->setPosition((double)npcPhysics->getPositionX(), (double)npcPhysics->getPositionY());

            if (isJumping)
            {
                npc->setAnimation("jumping", 0.2); // play jump animation
                if (listener.getTouchedBottom() > 0 && npcPhysics->getBody()->GetLinearVelocity().y == 0)
                {
                    if (player_direction) npc->setClip(idle_r);
                    else npc->setClip(idle_l);
                    isJumping = false;
                }
            }

            input->updateKeyboard();

            if (input->getKeyboardPressed("Right") && input->getKeyboardPressed("Left"))
            {
                npcPhysics->stopMoveHorizontal();
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
                npcPhysics->applyJump(-2.1);
            }

            if (input->getKeyboardReleased("Right"))
            {
                // set animation to idle, and velocity to 0
                if (!isJumping)
                    npc->setClip(idle_r);

                npcPhysics->stopMoveHorizontal();
            }
            if (input->getKeyboardReleased("Left") && !isJumping)
            {
                // set animation to idle, and velocity to 0
                if (!isJumping)
                    npc->setClip(idle_l);

                npcPhysics->stopMoveHorizontal();
            }

            _init->render();
            map->render(dt);
            npc->render(dt);

            text->setPosition(_init->getCameraX() + 0, 0);
            text->changeText(std::to_string(coin), false);
            text->render(dt);

            _init->updateCamera();
            _init->present();
        }

        void clean() override
        {
            _init->removeCamera();
            _init = NULL;
        }
};