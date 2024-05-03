#include "./asset_loader.h"
#include "./backend.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>

enum Movements {
    LEFT  = 1 << 0,
    RIGHT = 1 << 1,
    UP    = 1 << 2,
    DOWN  = 1 << 3,
};

#define TEXTURE_COUNT 5
#define FONT_COUNT 2

typedef struct Game {
    char  *title;
    int         texture_count;
    const char  *texture_indexes;
    Texture     **textures;

    int         music_count;
    const char  *music_indexes;
    Music       **music;

    int         sounds_count;
    const char  *sounds_indexes;
    SoundEffect **sounds;

    int         font_count;
    const char  *font_indexes;
    Font        **fonts;

    int         text_texture_count;
    const char  *text_texture_indexes;
    Texture     **text_textures;

    u8          move_direction;
} Game;
Game game = {0};

// @TODO(moosch): A lot of this should be handled by an asset manager.
void load_fonts(Game *game)
{
    game->font_count = 1;
    game->font_indexes = malloc(sizeof(int) * game->font_count);
    game->fonts = malloc(sizeof(Font*) * game->font_count);
    load_font(&game->fonts[0], "./assets/fonts/Ginger Cat.ttf", 50);
}

// @TODO(moosch): A lot of this should be handled by an asset manager.
void load_textures(Engine *engine, Game *game)
{
    game->texture_count = 5;
    game->texture_indexes = malloc(sizeof(int) * game->texture_count);
    game->textures = malloc(sizeof(Texture*) * game->texture_count);
    load_texture(engine->renderer, &game->textures[0], "./assets/images/background_1/platformer_background_1.png");

}

// @TODO(moosch): A lot of this should be handled by an asset manager.
void load_audio(Game *game)
{
    game->music_count = 1;
    game->music_indexes = malloc(sizeof(int) * game->music_count);
    game->music = malloc(sizeof(Music*) * game->music_count);
    load_music_file(&game->music[0], "./assets/audio/Sakura-Girl-Daisy.mp3");

    game->sounds_count = 1;
    game->sounds_indexes = malloc(sizeof(int) * game->sounds_count);
    game->sounds = malloc(sizeof(Music*) * game->sounds_count);
    load_sound_effect_file(&game->sounds[0], "./assets/audio/swipe-whoosh-1.wav");
}

void init_game(Engine *engine, Game *game)
{
    load_textures(engine, game);
    load_fonts(game);
    load_audio(game);
}

/* void handle_input(Game *game)
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
} */

void on_event(Event event)
{
    if (event.type == KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case KEY_p:
            toggle_music(game.music[0]);
            break;
        case KEY_s:
            stop_music();
            break;
        case KEY_space:
            play_sound_effect(game.sounds[0]);
            break;
        }
    }
}

void on_keypressed(const u8 *keyboard_state)
{
    if (keyboard_state[SDL_SCANCODE_RIGHT])
        game.move_direction += RIGHT;
    if (keyboard_state[SDL_SCANCODE_LEFT])
        game.move_direction += LEFT;
    if (keyboard_state[SDL_SCANCODE_UP])
        game.move_direction += UP;
    if (keyboard_state[SDL_SCANCODE_DOWN])
        game.move_direction += DOWN;
}

void reset(void)
{
    game.move_direction = 0;
}

void update(f32 delta_time)
{
    if (game.move_direction & RIGHT)
        printf("RIGHT\n");
    if (game.move_direction & LEFT)
        printf("LEFT\n");
    if (game.move_direction & UP)
        printf("UP\n");
    if (game.move_direction & DOWN)
        printf("DOWN\n");


    // game.move_speed * delta_time;

    // printf("%f\n", delta_time);
}

/**
 * Draw assets, fonts, images etc within this function.
 * Note the order of drawing.
 */ 
void draw(void)
{
    render_texture(game.textures[0], NULL, NULL);
    // render_text(game.text_textures[0], 10, 10);
}
 
void render(void)
{
}

// @TODO(moosch): This should be handled by an asset manager.
void cleanup(void)
{
    int i;
    for (i = 0; i < game.texture_count; ++i)
        if (game.textures[i] != NULL)
            SDL_DestroyTexture(game.textures[i]);

    for (i = 0; i < game.music_count; ++i)
        if (game.music[i] != NULL)
            Mix_FreeMusic(game.music[i]);

    for (i = 0; i < game.sounds_count; ++i)
        if (game.sounds[i] != NULL)
            Mix_FreeChunk(game.sounds[i]);
}

int main(void)
{
    Engine *engine = init_engine("SDL Engine Fun");

    init_game(engine, &game);

    // Color text_color = { 150, 100, 50, 255 };
    // engine.text = create_text(&engine, "Cheeky Kitty", text_color);

    setup_functions(reset, update, draw, render, on_event, on_keypressed, cleanup);

    run();

    /* f32 frame_time = 0.0f;
    u32 previous_time = 0;
    u32 current_time = 0;
    f32 delta_time = 0.0f;

    bool running = true;

    while(running)
    {
        previous_time = current_time;
        current_time = SDL_GetTicks(); // Number of ms since start of engine. 
        delta_time = (current_time - previous_time) / 1000.0f;

        handle_events(&engine, &running);

        frame_time += delta_time;

        // Only update screen on target FPS
        if (frame_time >= 0.25f)
        {
            reset(&engine);
            handle_input(&engine);
            update(&engine, delta_time);
            
            draw(&engine);

            render(&engine);
        }
    }
    cleanup(&engine); */

    return EXIT_SUCCESS;
}

