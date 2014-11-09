/* Global variables/function prototypes */

#ifndef MAIN_H
#define MAIN_H

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "timer.h"
#include "player.h"
#include "bullet.h"
#include "bad.h"
#include <string>

//The screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAMES_PER_SECOND = 30;

//The surfaces
SDL_Surface *dot = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *bulletSurface[NUM_BULLETS];
SDL_Surface *bad[NUM_BAD];
SDL_Surface *message = NULL;
SDL_Surface *titleScreen = NULL;

//The event structure
SDL_Event event;
//Wall(s)
// Number of walls is declared in player.h
SDL_Rect wall[NUM_WALLS];

//The font that's going to be used
TTF_Font *font = NULL;
//The color of the font
SDL_Color textColor = { 0, 0, 0 };
// Debug text
char debug[10] = "ab";

#endif