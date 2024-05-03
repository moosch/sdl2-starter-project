# A Game in C with SDL

This is a starter project for games in C using [SDL2](https://wiki.libsdl.org) for Window management, audio, images, fonts and more.

This will have you set up with basic asset loading of images, audio and fonts. A set of functions to render objects, update them over with with a `delta_time` float, and handle timing so that you get consistent frames-per-second on (almost) all machines.

**NOTE:**

This is a work in progress. I will be adding to it a lot and making improvements. However the basics are already there to start building a 2D game, without an engine. It's fun!

## Get Started

You will need the following SDL libraries ([found here](https://wiki.libsdl.org/SDL2/Libraries)) in your C compiler's expected library path:

- SDL2
- SDL_image
- SDL_mixer
- SDL_ttf

Run it with `./build.sh && ./game`.


## Todo

- [ ] Move assets (texture, fonts and audio) into the "engine" part of the code. The "game" can attach a key (maybe string for hashmap) to the asset in order to request it from the engine.
- [ ] Improve the keyboard events  that the "game" can access.
- [ ] Add support for controllers.
- [ ] Investigate hot-reloading of the game code. This will mean dynamically loading an `.so`, `.dll` or `.dylib` on Startup. The entrypoint will need to be the engine too, accepting the game as an arg or something.

## Asset Sources

### Music

[Daisy - Sakura Girl](https://www.chosic.com/download-audio/58136/)

[Run Amok - Kevin MacLeod](https://www.chosic.com/download-audio/39324/)

[Fluffing a Duck - Kevin MacLeod](https://www.chosic.com/download-audio/27248/)

[Movement Swipe Whoosh 1 - floraphonic](https://pixabay.com/users/floraphonic-38928062/)

[Movement Swipe Whoosh 2 - floraphonic](https://pixabay.com/users/floraphonic-38928062/)

[Movement Swipe Whoosh 3 - floraphonic](https://pixabay.com/users/floraphonic-38928062/)

[Atmospheric Metallic Swipe 17 - floraphonic](https://pixabay.com/users/floraphonic-38928062/)

### Images

[Cartoon Parallax](https://craftpix.net/freebies/free-cartoon-parallax-2d-backgrounds/)

### Fonts

[Ginger Cat](https://www.dafont.com/ginger-cat.font)

