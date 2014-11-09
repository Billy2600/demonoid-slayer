/*
	The badguy class, used for enemies
	Please note that both the declarations and definitions are in this file
	(There is no bad.cpp)
	The instance of the object is also declared here
	Original source code by Lazy Foo' Productions http://lazyfoo.net
*/
#ifndef BAD_H
#define BAD_H
#include "main.h"
#include "bullet.h"
#include "player.h"

//Width/height
const int BAD_WIDTH = 22;
const int BAD_HEIGHT = 32;
const int NUM_BAD = 7; // Max number of enemies
// Number of enemies killed
int NUM_BAD_KILLED;
// External variables from main.cpp
extern SDL_Surface *bad[];
extern SDL_Surface *screen;
const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
SDL_Rect clip[8];
extern bool check_collision(SDL_Rect,SDL_Rect);

class Bad
{
private:
	SDL_Rect box;
	int offSet;
	//animation status
	int status;
	//current frame
	int frame;
	//rate of movement
	int xVel, yVel;
	//direction: 1 up, 2 down, 3 left, 4 right
	int direction;
	// Number this badguy is
	int badNum;
	// Random velocity for this enemy
	int velocity;
	// Boolean if this enemy is dead
	bool dead;
	//Sprite clips
	SDL_Rect clipsRight[7];
	SDL_Rect clipsLeft[7];
	SDL_Rect clipsUp[7];
	SDL_Rect clipsDown[7];
public:
	Bad();
	// Change current position
	void changePos(int x, int y, int direct);
	// Move badguy
	void move();
	// Check collisions with bullets
	bool checkBulletCollisions();
	// Show badguiy on screen
	void show();
	// Change number of badguy
	void changeBadNum(int num);
	// Change badguy position (use for initialization, mostly)
	void changeBadPos(int x, int y);
	// 'Kill' this enemy
	void kill();
	// Check for collisons with other badguys
	bool checkBadCollisions();
	// Return box
	SDL_Rect returnBox() { return box; }
	// Respawn this enemy
	void respawn(int num);
};

Bad::Bad()
{
	// Get a random velocity
	velocity = rand() % (6 - 5 + 1) + 5;
	// Live damn you!
	dead = false;
	//Initialize offsets outside of screen
	box.x = 700;
	box.y = 700;
	box.h = BAD_HEIGHT;
	box.w = BAD_WIDTH;
	//Direction
	direction = 0;
	//Number of this Bad
	int BadNum = 1;
	frame = 0;
	offSet = 0;
	status = FOO_RIGHT;
	//Clip the sprites
	int getY = 0;
	for(int i = 0; i < 7; i++)
	{
		getY = 819 + ((BAD_HEIGHT + 95) * i); // Gets Y for each sprite going down the column

		clipsLeft[i].x = 567;
		clipsLeft[i].y = getY;
		clipsLeft[i].w = BAD_WIDTH;
		clipsLeft[i].h = BAD_HEIGHT;
		clipsRight[i].x = 1204;
		clipsRight[i].y = getY;
		clipsRight[i].w = BAD_WIDTH;
		clipsRight[i].h = BAD_HEIGHT;
	}
}

void Bad::changePos(int x, int y, int direct)
{
	//Initialize offsets
	box.x = x;
	box.y = y;
	box.h = BAD_HEIGHT;
	box.w = BAD_WIDTH;
	//Direction
	direction = direct;
}

void Bad::move()
{
	if(!dead) // Don't bother doing any of this if this enemy is dead
	{
		extern Bullet bullets[];
		// If hit by a bullet, 'kill' this enemy
		for(int i=0; i < NUM_BULLETS; i++)
		{
			if(check_collision(box,bullets[i].returnBox()))
			{
				kill();
				// 'Destroy' bullet that hit us
				bullets[i].destroy();
				// Add to number of badguys killed
				NUM_BAD_KILLED++;
			}
		}

		// Decide what direction we're going based on player and set velocity accordingly
		SDL_Rect playerPos;
		playerPos = myPlayer.returnBox();
		
		// Is the player right of bad?
		if(playerPos.x > box.x)
		{
			xVel = velocity;
		}
		// Is the player left of bad?
		else
		{
			xVel = velocity * -1; // Make it negative
		}
		// Is the player below bad?
		if(playerPos.y > box.y)
		{
			yVel = velocity;
		}
		// Is the player above bad?
		else
		{
			yVel = velocity * -1;
		}

		//Move the Bad, if not touching player or another badguy
		if(!check_collision(box,myPlayer.returnBox()))
		{
			box.x += xVel;
			box.y += yVel;
		}
		// If we're touching the player, hurt him
		else if(check_collision(box,myPlayer.returnBox()))
			myPlayer.hurtPlayer();
	}
}

void Bad::show()
{
	if(xVel < 0)
	{
		status = FOO_LEFT;

		frame++;
	}
	else if(xVel > 0)
	{
		status = FOO_RIGHT;

		frame++;
	}
	else
	{
		frame = 0;
	}
	//loop the animation
	if(frame >= 7)
	{
		frame = 0;
	}
	if(status == FOO_RIGHT)
	{
		apply_surface(box.x,box.y,bad[badNum],screen,&clipsRight[frame]);
	}
	else if( status == FOO_LEFT)
	{
		apply_surface(box.x,box.y,bad[badNum],screen,&clipsLeft[frame]);
	}
}

void Bad::changeBadNum(int num)
{
	badNum = num;
}

void Bad::changeBadPos(int x, int y)
{
	box.x = x;
	box.y = y;
}

void Bad::kill()
{
	// Move out of screen
	changeBadPos(700,700);
	// Make it not move
	dead = true;
}

bool Bad::checkBadCollisions()
{
	extern Bad badguy[];
	bool isTouching[NUM_BAD];
	//Check collisions
	for(int i=0; i < NUM_BAD; i++)
	{
		// Don't check outselves
		if(i != badNum)
			isTouching[i] = check_collision(box,badguy[i].returnBox());
		else
			isTouching[i] = false;
	}
	// Return collisions
	for(int i=0; i < NUM_BAD; i++)
	{
		if(isTouching[i])
			return true;
	}
	// No collisions, return false;
	return false;
}

void Bad::respawn(int num)
{
	int respX, respY;
	// Chose random respawn point
	int random = rand() % (5 - 1 + 1) + 1; 
	switch(random)
	{
	case 1: respX = 320; respY = 500; break;
	case 2: respX = 0; respY = 500; break;
	case 3: respX = 640; respY = 500; break;
	case 4: respX = 500; respY = 0; break;
	case 5: respX = 500; respY = 320; break;
	}
	changeBadPos(respX,respY+num);
	dead = false;
}

// Create instances of bad
Bad badguy[NUM_BAD];

#endif