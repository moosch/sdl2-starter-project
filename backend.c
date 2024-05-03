#include "./backend.h"
#include "defintes.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_scancode.h>
#include <stdio.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 650

Rect win_rect = {0};
bool is_running = false;

/*****************************************************************************
 * Setup Engine
 ****************************************************************************/

Engine* init_engine(const char *window_title)
{
    if (engine.window_w < 1)
        engine.window_w = DEFAULT_WINDOW_WIDTH;
    if (engine.window_h < 1)
        engine.window_h = DEFAULT_WINDOW_HEIGHT;

    win_rect.w = engine.window_w;
    win_rect.h = engine.window_h;
    engine.window_rect = &win_rect;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        printf("Error initialing SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() != 0)
    {
        printf("Error initialing SDL_TTF: %s\n", TTF_GetError());
        exit(1);
    }

    u32 img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags))
    {
        printf("Error initialing SDL_Image: %s\n", IMG_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    {
        printf("Error initialing SDL_Mixer: %s\n", Mix_GetError());
        exit(1);
    }

    engine.window = SDL_CreateWindow(window_title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    engine.window_w, engine.window_h,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    ASSERT(engine.window != NULL);

    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);
    ASSERT(engine.renderer != NULL);

    return &engine;
}


/*****************************************************************************
 * Register Hooks
 ****************************************************************************/

typedef void (*reset_p)(void);
typedef void (*update_p)(f32 delta_time);
typedef void (*draw_p)(void);
typedef void (*render_p)(void);
typedef void (*on_event_p)(Event event);
typedef void (*on_keypressed_p)(const u8 *keyboard_state);
typedef void (*cleanup_p)(void);
typedef struct Game_Fns {
    reset_p   reset;
    update_p  update;
    draw_p    draw;
    render_p  render;

    // Events
    on_event_p      on_event;
    on_keypressed_p on_keypressed;

    // On quit
    cleanup_p cleanup;
} Game_Fns;
Game_Fns game_fns = {};

void _reset(void)
{
    game_fns.reset();
    SDL_RenderClear(engine.renderer);
}
void _update(f32 delta_time)
{
    game_fns.update(delta_time);
}
void _draw(void)
{
    game_fns.draw();
}
void _render(void)
{
    game_fns.render();

    SDL_RenderPresent(engine.renderer);
}
void _on_event(void)
{
    Event event;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            is_running = false;
            SDL_Quit();
        }

        game_fns.on_event(event);
    }
}
void _on_keypressed(void)
{
    // @TODO(moosch): This needs to be split. Event above outside of rendering frame_time check
    // Then send the below events from within that `if`

    game_fns.on_keypressed(SDL_GetKeyboardState(NULL));
}

void _cleanup()
{
    game_fns.cleanup();

    SDL_DestroyRenderer(engine.renderer);
    SDL_DestroyWindow(engine.window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void setup_functions(void (*reset_p)(void),
                     void (*update_p)(f32 delta_time),
                     void (*draw_p)(void),
                     void (*render_p)(void),
                     void (*on_event_p)(Event event),
                     void (*on_keypressed_p)(const u8 *keyboard_state),
                     void (*cleanup_p)(void))
{
    game_fns.reset = reset_p;
    game_fns.update = update_p;
    game_fns.draw = draw_p;
    game_fns.render = render_p;
    game_fns.on_event = on_event_p;
    game_fns.on_keypressed = on_keypressed_p;
    game_fns.cleanup = cleanup_p;
}

void run()
{
    f32 frame_time = 0.0f;
    u32 previous_time = 0;
    u32 current_time = 0;
    f32 delta_time = 0.0f;

    is_running = true;

    while(is_running)
    {
        previous_time = current_time;
        current_time = SDL_GetTicks(); // Number of ms since start of engine. 
        delta_time = (current_time - previous_time) / 1000.0f;

        frame_time += delta_time;

        _on_event();

        // Only update screen on target FPS
        if (frame_time >= 0.25f)
        {
            _on_keypressed();

            _reset();

            _update(delta_time);
            
            _draw();

            _render();
        }
    }
    _cleanup();
}


/*****************************************************************************
 * Resource Management
 ****************************************************************************/

/**
 * Returns a pointer (Texture*) of text in a supplied font and color.
 * ```c
 * Color color = {100, 100, 100, 255};
 * Texture *text = create_text(&font, "Some text", color);
 * ```
 */
Texture* create_text(Font *font, const char *text, Color color)
{
    Surface *surface = TTF_RenderText_Solid(font, text, color);
    Texture *text_texture = SDL_CreateTextureFromSurface(engine.renderer, surface);

    SDL_FreeSurface(surface);
    return text_texture;
}

void toggle_music(Music *music)
{
    if (!Mix_PlayingMusic())
        Mix_PlayMusic(music, -1);
    else if (Mix_PausedMusic())
        Mix_ResumeMusic();
    else
        Mix_PauseMusic();
}
void stop_music(void)
{
    Mix_HaltMusic();
}

void play_sound_effect(SoundEffect *sound)
{
    Mix_PlayChannel(-1, sound, 0);
}





/*****************************************************************************
 * Rendering
 ****************************************************************************/

/**
 * Renders text Texture at window coordinates.
 * Usage:
 * ```c
 * Color color = {100, 100, 100, 255};
 * Texture *text = create_text("Some text!", color);
 * render_text(text, 100, 150);
 * ```
 */
void render_text(Texture *text, u32 pos_x, u32 pos_y)
{
    Rect rect = {pos_x, pos_y, 0, 0};

    SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(engine.renderer, text, NULL, &rect);
}

/**
 * Renders a Texture with optional src and dest Rects.
 * Usage:
 * ```c
 * render_texture(texture, NULL, NULL);
 * render_texture(texture, NULL, dest);
 * render_texture(texture, src, dest);
 * ```
 */
void render_texture(Texture *text, Rect *src, Rect *dest)
{
    SDL_RenderCopy(engine.renderer, text, src, dest);
}



