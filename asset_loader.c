#include "./asset_loader.h"

void load_font(Font **font, const char *path, size_t size)
{
    *font = TTF_OpenFont(path, size);
}

void load_texture(Renderer *renderer, Texture **texture, const char *path)
{
    // Surface *surface = SDL_LoadBMP(path);
    Surface *surface = IMG_Load(path);

    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    ASSERT(texture != NULL);

    SDL_FreeSurface(surface);
}

void load_music_file(Music **audio, const char *path)
{
    *audio = Mix_LoadMUS(path);
}

void load_sound_effect_file(SoundEffect **audio, const char *path)
{
    *audio = Mix_LoadWAV(path);
}


