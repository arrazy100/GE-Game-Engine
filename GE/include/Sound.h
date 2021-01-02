#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>
#include <string>

namespace GE
{
    class Sound
    {
        private:
            Mix_Chunk* _sound_effect = NULL;
            Mix_Music* _music = NULL;
        public:
            Sound();
            ~Sound();
            void loadSoundEffect(std::string file);
            void freeSoundEffect();
            void changeSoundEffect(std::string file);
            void loadMusic(std::string file);
            void freeMusic();
            void changeMusic(std::string file);
            void changeVolume(int percentage);
            void play(int loop);
            void pause();
            void resume();
            bool isPlaying();
            bool isPaused();
    };
}

#endif