/*
	The player class, used to move and control the player
	Please note that both the declarations and definitions are in this file
	(There is no Player.cpp)
	The instance of the object is also declared here
	Original source code by Lazy Foo' Productions http://lazyfoo.net
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "bullet.h"

//Get all of our values/functions declared in main.cpp
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern void apply_surface(int,int,SDL_Surface*,SDL_Surface*,SDL_Rect*);
extern bool check_collision(SDL_Rect,SDL_Rect);

// Walls and number of walls
extern SDL_Rect wall[];
const int NUM_WALLS = 4;

//The dimensions of the player
const int PLAYER_WIDTH = 23;
const int PLAYER_HEIGHT = 26;

class Player
{
    private:
	//The collision box of the square
    SDL_Rect box;
    //The velocity of the dot
    int xVel, yVel;
	// player direction: 1 up, 2 down, 3 left, 4 right
	int playerDirection;
	bool up, down, left, right;
	// Current sprite frame
	int frame;
	// Current player health
	int hp;
	// Current bullet
	int curBullet;
	//Sprite clips
	SDL_Rect clipsRight[8];
	SDL_Rect clipsLeft[8];
	SDL_Rect clipsUp[8];
	SDL_Rect clipsDown[8];
	// Post getting hit invunerability stuff
	bool hurt; // Boolean
	Uint32 hurtStart; // When we first got hurt
	Uint32 blinkStart; // Start timer for blinking

    public:
    //Initializes the variables
    Player();
    //Takes key presses and adjusts the player's velocity
    void handle_input();
    //Moves the player
    void move();
	// Check for collisions with walls
	bool checkWallCollisions();
    //Shows the player on the screen
    void show();
	// Hurt the player, called by enemy that touched us
	void hurtPlayer();
	// Return collision box
	SDL_Rect returnBox() { return box; }
	// Return current hit points
	int returnHp() { return hp; }
	// Return xVel
	int returnXVel() { return xVel; }
	// return yVel
	int returnYVel() { return yVel; }
};


Player::Player()
{
    //Initialize the offsets
    box.x = 70;
    box.y = 70;
	//Set the player's dimentions
    box.w = PLAYER_WIDTH;
    box.h = PLAYER_HEIGHT;
	playerDirection = 4;
	up=false, down=false, left=false, right=false;
	// Set hitpoints
	hp = 100;
	curBullet = 0;
	hurt = false;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;

	//Clip the sprites
	int getY = 0;
	for(int i = 0; i < 8; i++)
	{
		getY = 275 + ((PLAYER_HEIGHT + 38) * i); // Gets Y for each sprite going down the column

		clipsUp[i].x = 20;
		clipsUp[i].y = getY;
		clipsUp[i].w = PLAYER_WIDTH;
		clipsUp[i].h = PLAYER_HEIGHT;
		clipsDown[i].x = 920;
		clipsDown[i].y = getY;
		clipsDown[i].w = PLAYER_WIDTH;
		clipsDown[i].h = PLAYER_HEIGHT;
		clipsLeft[i].x = 469;
		clipsLeft[i].y = getY;
		clipsLeft[i].w = PLAYER_WIDTH;
		clipsLeft[i].h = PLAYER_HEIGHT;
		clipsRight[i].x = 596;
		clipsRight[i].y = getY;
		clipsRight[i].w = PLAYER_WIDTH;
		clipsRight[i].h = PLAYER_HEIGHT;
	}
}

void Player::handle_input()
{
	extern SDL_Event event;
	extern Bullet bullets[];
	extern const int NUM_BULLETS;
	int bulDirection; // Direction we'll fire the bullet
	const int speed = 8; // number that PLAYER_HEIGHT is devided by to get velocity
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel -= PLAYER_HEIGHT / speed; playerDirection = 1; up=true; down=false; break;
            case SDLK_DOWN: yVel += PLAYER_HEIGHT / speed; playerDirection = 2; down=true; up=false; break;
            case SDLK_LEFT: xVel -= PLAYER_WIDTH / speed; playerDirection = 3; left=true; right=false; break;
            case SDLK_RIGHT: xVel += PLAYER_WIDTH / speed; playerDirection = 4; right=true; left=false; break;
			// Fire bullet
			case SDLK_SPACE:
				// Decide what direction the bullet needs to go (including diagonals)
				if(up && left) // Up left;
					bulDirection = 5;
				else if(up && right) // Up right
					bulDirection = 6;
				else if(down && left) // Down left
					bulDirection = 7;
				else if(down && right) // Down right
					bulDirection = 8;
				else // Regular directions
					bulDirection = playerDirection;
				// Fire the bullet
				bullets[curBullet].fire((box.x+box.w)-(box.w/2),((box.y+box.h)-(box.h/2)),bulDirection);
				// Go to next bullet
				curBullet++;
				// If we reach the max, go back to the first
				if(curBullet == NUM_BULLETS)
  					curBullet = 0;
			break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel += PLAYER_HEIGHT / speed; up=false; break;
            case SDLK_DOWN: yVel -= PLAYER_HEIGHT / speed; down=false; break;
            case SDLK_LEFT: xVel += PLAYER_WIDTH / speed; left=false; break;
            case SDLK_RIGHT: xVel -= PLAYER_WIDTH / speed; right=false; break;
        }
    }
}

void Player::move()
{
    //Move the dot left or right
    box.x += xVel;

    //If the dot went too far to the left or right
    if( ( box.x <= 0 ) || ( box.x + PLAYER_WIDTH > SCREEN_WIDTH ) || checkWallCollisions())
    {
        //move back
        box.x -= xVel;
    }

    //Move the dot up or down
    box.y += yVel;

    //If the dot went too far up or down
    if( ( box.y <= 0 ) || ( box.y + PLAYER_HEIGHT > SCREEN_HEIGHT) || checkWallCollisions())
    {
        //move back
        box.y -= yVel;
    }

	// Check if we're done being hurt (4 seconds have past since hurt)
	if(hurt == true)
		if((SDL_GetTicks() - hurtStart)/1000 >= 4)
			hurt = false;
}

bool Player::checkWallCollisions()
{
	bool isTouching[NUM_WALLS];
	//Check collisions
	for(int i=0; i < NUM_WALLS; i++)
	{
		isTouching[i] = check_collision(box,wall[i]);
	}
	// Return collisions
	for(int i=0; i < NUM_WALLS; i++)
	{
		if(isTouching[i])
			return true;
	}
	// No collisions, return false;
	return false;
}

void Player::show()
{
	extern SDL_Surface *dot;
	extern SDL_Surface *screen;

    //Player is moving
    if(xVel > 0 || xVel < 0 || yVel > 0 || yVel < 0)
	{
        frame++;
    }
    //If player is standing
    else
    {
        //Restart the animation
        frame = 0;
    }

    //Loop the animation
    if( frame >= 8 )
    {
        frame = 0;
	}

	// Check if we need to blink
	bool blink=false;
	if(hurt)
	{
		// Make character blink based on time elapsed
		Uint32 now = SDL_GetTicks(); 
		if(now - blinkStart >= 100)
		{
			blinkStart = now; // Update blink start to now
			blink = !blink; // Reverse blink bool
		}
	}
	else
		blink = false;

	// do not draw sprites if we're blinking
	if(!blink)
	{
		//Show the player, based on direction
		switch(playerDirection)
		{
		case 1:
			apply_surface(box.x,box.y,dot,screen,&clipsUp[frame]);
			break;
		case 2:
			apply_surface(box.x,box.y,dot,screen,&clipsDown[frame]);
			break;
		case 3:
			apply_surface(box.x,box.y,dot,screen,&clipsLeft[frame]);
			break;
		default:
			apply_surface(box.x,box.y,dot,screen,&clipsRight[frame]);
			break;
		}
	}
}

void Player::hurtPlayer()
{
	// Skip all of this if we're already hurt
	if(!hurt)
	{
		// Set hurt boolean
		hurt = true;
		// Subtract from health
		hp = hp - 12;
		// Start timers
		hurtStart = SDL_GetTicks();
		blinkStart = SDL_GetTicks();
	}
}

// Create player instance
Player myPlayer;

#endif