/*
	The bullet class, used for bullets
	Please note that both the declarations and definitions are in this file
	(There is no bullet.cpp)
	The instance of the object is also declared here
	Original source code by Lazy Foo' Productions http://lazyfoo.net
*/
#ifndef BULLET_H
#define BULLET_H

#include "main.h"

//Width/height
const int BULLET_WIDTH = 8;
const int BULLET_HEIGHT = 8;
const int NUM_BULLETS = 10; // Max number of bullets
// External variables/functions from main.cpp
extern SDL_Surface *bulletSurface[];
extern SDL_Surface *screen;
extern void apply_surface(int,int,SDL_Surface*,SDL_Surface*,SDL_Rect*);

class Bullet
{
private:
	SDL_Rect box;
	int bulletNum; // Number of this bullet
	int xVel, yVel;
	//direction: 1 up, 2 down, 3 left, 4 right
	//		5 up left, 6 up right, 7 down left, 8 down right
	int direction;
public:
	Bullet();
	// Fire bullet (changes position/direction)
	void fire(int x, int y, int direct);
	// Change the number of this bullet (used for initializing)
	void changeBulNum(int num);
	// Move bullet based on direction
	void move();
	// Show bullet
	void show();
	// Return collision box
	SDL_Rect returnBox();
	// "Destroy" this bullet - moves it offscreen and halts it
	void destroy();
};

Bullet::Bullet()
{
	//Initialize offsets, outside of screen
	box.x = 650;
	box.y = 490;
	box.h = BULLET_HEIGHT;
	box.w = BULLET_WIDTH;
	//Direction, not moving
	direction = 0;
	//Number of this bullet
	int bulletNum = 0;
}

void Bullet::changeBulNum(int num)
{
	bulletNum = num;
}

void Bullet::fire(int x, int y, int direct)
{
	// Change offsets
	box.x = x;
	box.y = y;
	//Direction
	direction = direct;
}

void Bullet::move()
{
	extern const int SCREEN_WIDTH, SCREEN_HEIGHT;
	int velocity = 12; // Velocity we're going to add or subtract
	// Decide what direction we're going and set velocity accordingly
	switch(direction)
	{
	//Up
	case 1:
		yVel = velocity * -1;
		xVel = 0;
		break;
	//Down
	case 2:
		yVel = velocity;
		xVel = 0;
		break;
	//Left
	case 3:
		xVel = velocity * -1;
		yVel = 0;
		break;
	//Right
	case 4:
		xVel = velocity;
		yVel = 0;
		break;
	// Up Left
	case 5:
		xVel = velocity * -1;
		yVel = velocity * -1;
		break;
	// Up right
	case 6:
		xVel = velocity;
		yVel = velocity * -1;
		break;
	// Down left
	case 7:
		xVel = velocity * -1;
		yVel = velocity;
		break;
	// Down right
	case 8:
		xVel = velocity;
		yVel = velocity;
		break;
	// Not moving
	default:
		xVel = 0;
		yVel = 0;
		break;
	}
	//Move the bullet, until it hits the edge
	if((box.x <= 0) || (box.x + BULLET_WIDTH > SCREEN_WIDTH) || (box.y <= 0) || (box.y + BULLET_HEIGHT > SCREEN_HEIGHT))
	{
		destroy();
	}
	else
	{
		box.x += xVel;
		box.y += yVel;
	}
}

void Bullet::show()
{
	apply_surface(box.x,box.y,bulletSurface[bulletNum],screen,NULL);
}

SDL_Rect Bullet::returnBox()
{
	return box;
}

void Bullet::destroy()
{
	box.x = 10000;
	box.y = 10000;
	direction = 0;
}

// Create bullet objects
Bullet bullets[NUM_BULLETS];

#endif