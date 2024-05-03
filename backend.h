#ifndef _MOOSCH_BACKEND_H_
#define _MOOSCH_BACKEND_H_

#include "./defintes.h"

static Engine engine;

Engine* init_engine(const char *wineod_title);

/**
 * Returns a pointer (Texture*) of text in a supplied font and color.
 * ```c
 * Color color = {100, 100, 100, 255};
 * Texture *text = create_text(engine, "Some text", color);
 * ```
 */
Texture* create_text(Font *font, const char *text, Color color);

/**
 * Renders text Texture at window coordinates.
 * Usage:
 * ```c
 * Color color = {100, 100, 100, 255};
 * Texture *text = create_text(engine, "Some text!", color);
 * render_text(engine, text, 100, 150);
 * ```
 */
void render_text(Texture *text, u32 pos_x, u32 pos_y);

/**
 * Renders a Texture with optional src and dest Rects.
 * Usage:
 * ```c
 * render_texture(engine, texture, NULL, NULL);
 * render_texture(engine, texture, NULL, dest);
 * render_texture(engine, texture, src, dest);
 * ```
 */
void render_texture(Texture *text, Rect *src, Rect *dest);



void toggle_music(Music *music);
void stop_music(void);
void play_sound_effect(SoundEffect *sound);

/**
 * Define and pass in your game loop functions.
 * Usage:
 * ```c
 * void reset(void)
 * {
 *     // Reset some state
 * }
 * void update(f32 delta_time)
 * {
 *     // Update some state
 *     game.player.position_x += 100 * delta_time;
 * }
 * void draw(void)
 * {
 *     // Render objects in specific order
 *     render_text(game.text, 50, 50);
 * }
 * void render(void)
 * {
 *     // Perform additional tasks after rendering is complete
 *     // e.g. Cleanup unrequired resources.
 *     destroy_texture(game.texture);
 * }
 * void cleanup(void)
 * {
 *     // Free any allocated resources when game quits
 *     destroy_texture(game.texture);
 *     destroy_music(game.music);
 * }
 * ```
 */
void setup_functions(void (*reset_p)(void),
                     void (*update_p)(f32 delta_time),
                     void (*draw_p)(void),
                     void (*render_p)(void),
                     void (*on_event_p)(Event event),
                     void (*on_keypressed_p)(const u8 *keyboard_state),
                     void (*cleanup_p)(void));

/**
 * Starts the game loop.
 */
void run();

#endif

