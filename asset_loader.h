#ifndef _MOOSCH_ASSET_LOADER_H_
#define _MOOSCH_ASSET_LOADER_H_

#include "./defintes.h"
#include "./backend.h"

/**
 * Returns a pointer (Font*) from a loaded font.
 * Usage:
 * ```c
 * Font font;
 * load_font(&font, "./assets/fonts/some_font.ttf");
 * ```
 */
void load_font(Font **font, const char *path, size_t size);

/**
 * Returns a pointer (Texture*) from loaded texture.
 * Usage:
 * ```c
 * Texture texture;
 * load_texture(&texture, "./assets/images/some_image.bmp");
 * ```
 */
void load_texture(Renderer *renderer, Texture **texture, const char *path);

/**
 * Returns a pointer (Music*) from an mp3 file.
 * ```c
 * Usage:
 * Music music;
 * load_music_file(&music, "./assets/audio/some_music.mp3");
 * ```
 */
void load_music_file(Music **music, const char *path);

/**
 * Returns a pointer (SoundEffect*) from an audio wav file.
 * Usage:
 * ```c
 * SoundEffect audio;
 * load_sound_effect(&audio, "./assets/audio/some_sound.wav");
 * ```
 */
void load_sound_effect_file(SoundEffect **audio, const char *path);

#endif

