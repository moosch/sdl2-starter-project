#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define ASSERT(expr) \
    if ((expr)) \
    {} \
    else \
        printf("AssertErro on line %d\n\t\t%s failed.\n", __LINE__, #expr) \

#define WIN_WIDTH 600
#define WIN_HEIGHT 450

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float_t  f32;

enum Movements {
    LEFT  = 1 << 0,
    RIGHT = 1 << 1,
    UP    = 1 << 2,
    DOWN  = 1 << 3,
};

typedef SDL_Texture  Texture;
typedef SDL_Surface  Surface;
typedef SDL_Renderer Renderer;
typedef SDL_Window   Window;
typedef SDL_Color    Color;
typedef SDL_Event    Event;
typedef SDL_Rect     Rect;
typedef TTF_Font     Font;
typedef Mix_Music    Music;
typedef Mix_Chunk    SoundEffect;

typedef struct Vec3 {
    u32 x;
    u32 y;
    u32 z;
} Vec3;

typedef struct Vec4 {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
} Vec4;

typedef struct Game {
    Window      *window;
    Renderer    *renderer;
    char        *title;
    f32         move_speed;
    const u8    *keyboard_state;
    u8          move_direction;

    Texture     *text;

    Texture     *texture;
    Font        *font;

    Music       *music;
    SoundEffect *swipe_1;
} Game;
Rect window_rect = {0, 0, WIN_WIDTH, WIN_HEIGHT};

void load_font(Game *game, const char *path, size_t size)
{
    game->font = TTF_OpenFont(path, size);
}

void load_fonts(Game *game)
{
    load_font(game, "./assets/fonts/Ginger Cat.ttf", 50);
}

/**
 * Creates text surface.
 * Usage:
 * ```c
 * Color color = {100, 100, 100, 255};
 * Texture *text = create_text(game, "Some text!", color);
 * ```
 */
Texture *create_text(Game *game, const char *text, Color color)
{
    Surface *surface = TTF_RenderText_Solid(game->font, text, color);
    Texture *text_texture = SDL_CreateTextureFromSurface(game->renderer, surface);

    SDL_FreeSurface(surface);
    return text_texture;
}

/**
 * Renders text Texture at window coordinates.
 * Usage:
 * ```c
 * Color color = {100, 100, 100, 255};
 * Texture *text = create_text(game, "Some text!", color);
 * render_text(game, text, 100, 150);
 * ```
 */
void render_text(Game *game, Texture *text, u32 pos_x, u32 pos_y)
{
    Rect rect = {pos_x, pos_y, 0, 0};

    SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(game->renderer, text, NULL, &rect);
}

void render_texture(Game *game, Texture *text, Rect *src, Rect *dest)
{
    SDL_RenderCopy(game->renderer, game->texture, src, dest);
}

/**
 * Returns an SDL_Texture from a bitmap.
 * Usage:
 * ```c
 * Texture *img = load_texture(&game, "./assets/images/some_image.bmp");
 * ```
 */
Texture *load_texture(Game *game, const char *path)
{
    Texture *texture;
    // Surface *surface = SDL_LoadBMP(path);
    Surface *surface = IMG_Load(path);

    texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    ASSERT(texture != NULL);

    SDL_FreeSurface(surface);

    return texture;
}

void load_textures(Game *game)
{
    game->texture = load_texture(game, "./assets/images/background_1/platformer_background_1.png");
}

Music *load_music_file(const char *path)
{
    Music *audio = Mix_LoadMUS(path);

    return audio;
}
SoundEffect *load_sound_effect_file(const char *path)
{
    SoundEffect *audio = Mix_LoadWAV(path);

    return audio;
}
void load_audio(Game *game)
{
    game->music = load_music_file("./assets/audio/Sakura-Girl-Daisy.mp3");
    game->swipe_1 = load_sound_effect_file("./assets/audio/swipe-whoosh-1.wav");
}

void toggle_music(Game *game)
{
    if (!Mix_PlayingMusic())
        Mix_PlayMusic(game->music, -1);
    else if (Mix_PausedMusic())
        Mix_ResumeMusic();
    else
        Mix_PauseMusic();
}
void stop_music(Game *game)
{
    Mix_HaltMusic();
}
void play_sound_effect(Game *game)
{
    Mix_PlayChannel(-1, game->swipe_1, 0);
}

void reset(Game *game)
{
    SDL_RenderClear(game->renderer);

    game->move_direction = 0;
}

void handle_input(Game *game)
{
    game->move_direction = 0;
    game->keyboard_state = SDL_GetKeyboardState(NULL);

    if (game->keyboard_state[SDL_SCANCODE_RIGHT])
        game->move_direction += RIGHT;
    if (game->keyboard_state[SDL_SCANCODE_LEFT])
        game->move_direction += LEFT;
    if (game->keyboard_state[SDL_SCANCODE_UP])
        game->move_direction += UP;
    if (game->keyboard_state[SDL_SCANCODE_DOWN])
        game->move_direction += DOWN;
}

void init_sdl(Game *game)
{
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

    game->window = SDL_CreateWindow(game->title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WIN_WIDTH, WIN_HEIGHT,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    ASSERT(game->window != NULL);

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    ASSERT(game->renderer != NULL);
}

void handle_events(Game *game, bool *running)
{
    Event event;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            *running = false;
            SDL_Quit();
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_p:
                toggle_music(game);
                break;
            case SDLK_s:
                stop_music(game);
                break;
            case SDLK_SPACE:
                play_sound_effect(game);
                break;
            }
        }
    }
}

void update(Game *game, f32 delta_time)
{
    if (game->move_direction & RIGHT)
        printf("RIGHT\n");
    if (game->move_direction & LEFT)
        printf("LEFT\n");
    if (game->move_direction & UP)
        printf("UP\n");
    if (game->move_direction & DOWN)
        printf("DOWN\n");


    // game->move_speed * delta_time;

    // printf("%f\n", delta_time);
}

/**
 * Draw assets, fonts, images etc within this function.
 * Note the order of drawing.
 */
void draw(Game *game)
{
    render_texture(game, game->texture, NULL, NULL);
    render_text(game, game->text, 10, 10);
}

void render(Game *game)
{
    SDL_RenderPresent(game->renderer);
}

void cleanup(Game *game)
{
    if (game->texture != NULL)
        SDL_DestroyTexture(game->texture);

    if (game->renderer != NULL)
        SDL_DestroyRenderer(game->renderer);

    if (game->window != NULL)
        SDL_DestroyWindow(game->window);

    if (game->music != NULL)
        Mix_FreeMusic(game->music);
    if (game->swipe_1 != NULL)
        Mix_FreeChunk(game->swipe_1);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(void)
{
    Game game = {0};
    game.move_speed = 100.0f;

    game.title = "SDL Game Fun";
    init_sdl(&game);

    load_textures(&game);
    load_fonts(&game);
    load_audio(&game);

    Color text_color = { 150, 100, 50, 255 };
    game.text = create_text(&game, "Cheeky Kitty", text_color);

    f32 frame_time = 0.0f;
    u32 previous_time = 0;
    u32 current_time = 0;
    f32 delta_time = 0.0f;

    bool running = true;

    while(running)
    {
        previous_time = current_time;
        current_time = SDL_GetTicks(); // Number of ms since start of game.
        delta_time = (current_time - previous_time) / 1000.0f;

        handle_events(&game, &running);

        frame_time += delta_time;

        // Only update screen on target FPS
        if (frame_time >= 0.25f)
        {
            reset(&game);
            handle_input(&game);
            update(&game, delta_time);
            
            draw(&game);

            render(&game);
        }
    }
    cleanup(&game);

    return EXIT_SUCCESS;
}

