#ifndef _MOOSCH_DEFINES_H_
#define _MOOSCH_DEFINES_H_

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

#include <bits/types.h>
#include <stdio.h>
#include <stdbool.h>

// A safe assert macro in case there's an `else` directly afterward.
#define ASSERT(expr) \
    if ((expr)) \
    {} \
    else \
        printf("AssertError on line %d\n\t\t%s failed.\n", __LINE__, #expr) \

typedef __uint8_t  u8;
typedef __uint16_t u16;
typedef __uint32_t u32;
typedef __uint64_t u64;
typedef __int8_t   i8;
typedef __int16_t  i16;
typedef __int32_t  i32;
typedef __int64_t  i64;
typedef float      f32;

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

typedef enum
{
    KEYDOWN      = SDL_KEYDOWN,
    KEYUP        = SDL_KEYUP,
    WINDOW_EVENT = SDL_WINDOWEVENT,
} EventType;

typedef enum
{
    KEY_a = 'a',
    KEY_b = 'b',
    KEY_c = 'c',
    KEY_d = 'd',
    KEY_e = 'e',
    KEY_f = 'f',
    KEY_g = 'g',
    KEY_h = 'h',
    KEY_i = 'i',
    KEY_j = 'j',
    KEY_k = 'k',
    KEY_l = 'l',
    KEY_m = 'm',
    KEY_n = 'n',
    KEY_o = 'o',
    KEY_p = 'p',
    KEY_q = 'q',
    KEY_r = 'r',
    KEY_s = 's',
    KEY_t = 't',
    KEY_u = 'u',
    KEY_v = 'v',
    KEY_w = 'w',
    KEY_x = 'x',
    KEY_y = 'y',
    KEY_z = 'z',
    
    KEY_down  = SDLK_DOWN,
    KEY_up    = SDLK_UP,
    KEY_left  = SDLK_LEFT,
    KEY_right = SDLK_RIGHT,
    
    KEY_esc       = SDLK_ESCAPE,
    KEY_space     = SDLK_SPACE,
    KEY_return    = SDLK_RETURN,
    KEY_backspace = SDLK_BACKSPACE,
} KeyCode;

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

typedef struct Engine {
    u32         window_w;
    u32         window_h;
    Rect        *window_rect;

    Window      *window;
    Renderer    *renderer;
    f32         move_speed;
    const u8    *keyboard_state;
} Engine;

#endif

