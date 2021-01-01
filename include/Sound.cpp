#include "Sound.h"

GE::Sound::Sound()
{

}

GE::Sound::~Sound()
{
    freeSoundEffect();
    freeMusic();
}

void GE::Sound::loadSoundEffect(std::string file)
{
    if (_music) freeMusic();
    else if (_sound_effect) freeSoundEffect();

    _sound_effect = Mix_LoadWAV(file.c_str());
    if (!_sound_effect)
    {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
}

void GE::Sound::freeSoundEffect()
{
    Mix_FreeChunk(_sound_effect);
    _sound_effect = NULL;
}

void GE::Sound::changeSoundEffect(std::string file)
{
    Mix_FreeChunk(_sound_effect);
    loadSoundEffect(file);
}

void GE::Sound::changeVolume(int percentage)
{
    if (_sound_effect)
    {
        Mix_VolumeChunk(_sound_effect, MIX_MAX_VOLUME * percentage);
    }
    else if (_music)
    {
        Mix_VolumeMusic(MIX_MAX_VOLUME * percentage);
    }
}

void GE::Sound::loadMusic(std::string file)
{
    if (_sound_effect) freeSoundEffect();
    else if (_music) freeMusic();

    _music = Mix_LoadMUS(file.c_str());
    if (!_music)
    {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
}

void GE::Sound::freeMusic()
{
    Mix_FreeMusic(_music);
    _music = NULL;
}

void GE::Sound::changeMusic(std::string file)
{
   freeMusic();
    loadMusic(file);
}

void GE::Sound::play(int loop)
{
    if (_sound_effect)
    {
        if (Mix_PlayChannel(-1, _sound_effect, loop) == -1) {
            printf("Mix_PlayChannel: %s\n", Mix_GetError());
        }
    }
    else if (_music)
    {
        if (Mix_PlayMusic(_music, loop) == -1) {
            printf("Mix_PlayMusic: %s\n", Mix_GetError());
        }
    }
}

void GE::Sound::pause()
{
    if (_sound_effect)
    {
        Mix_Pause(-1);
    }
    else if (_music)
    {
        Mix_PauseMusic();
    }
}

void GE::Sound::resume()
{
    if (_sound_effect)
    {
        Mix_Resume(-1);
    }
    else if (_music)
    {
        Mix_ResumeMusic();
    }
}

bool GE::Sound::isPlaying()
{
    if (_sound_effect)
    {
        return Mix_Playing(-1);
    }
    else if (_music)
    {
        return Mix_PlayingMusic();
    }
}

bool GE::Sound::isPaused()
{
    if (_sound_effect)
    {
        return Mix_Paused(-1);
    }
    else if (_music)
    {
        return Mix_PausedMusic();
    }
}