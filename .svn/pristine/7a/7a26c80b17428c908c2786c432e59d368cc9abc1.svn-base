/*
	main.cpp - The location of the main function, and other misc functions
	This file is a good place to start reading
	Original source code by Lazy Foo' Productions http://lazyfoo.net
	The original code used a 'dot' instead of a 'player', so if you see references to dots,
	it's really talking about the player
*/

//The headers
#include "main.h"

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset);
}

//Check for collisions
bool check_collision( SDL_Rect A, SDL_Rect B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

	//Initialize SDL_ttf
	if(TTF_Init() == -1)
	{ 
		return false;
	}

	// Make all bullet surfaces null for now
	for(int i = 0; i < NUM_BULLETS; i++)
	{
		bulletSurface[i] = NULL;
		bullets[i].changeBulNum(i);
	}
	// Do the same above, except for enemy surfaces
	for(int i = 0; i < NUM_BAD; i++)
	{
		bad[i] = NULL;
		badguy[i].changeBadNum(i);
		badguy[i].respawn(i);
	}

    //Set the window caption
    SDL_WM_SetCaption( "Demonoid Slayer", NULL );

	NUM_BAD_KILLED = 0;

    //If everything initialized fine
    return true;
}

bool load_files()
{
	// Load the title screen
	titleScreen = load_image( "img/title_screen.bmp" );
    //Load the dot image
    dot = load_image( "img/marinez.bmp" );
	//Load the background
	background = load_image("img/dirtbg.bmp");
	//Load bullet sprite
	for(int i = 0; i < NUM_BULLETS; i++)
		bulletSurface[i] = load_image("img/bullet.bmp");
	//Load enemy sprite
	for(int i = 0; i < NUM_BAD; i++)
		bad[i] = load_image("img/zealotx.bmp");
	// Load font
	font = TTF_OpenFont( "font.ttf", 28 );

    //If there was a problem in loading the images
	if(titleScreen == NULL)
		return false;
    if( dot == NULL )
        return false;
	if(background == NULL)
		return false;
	if(bulletSurface[0] == NULL)
		return false;
	if(bad[0] == NULL)
		return false;
	if(font == NULL)
		return false;
    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface(dot);
	SDL_FreeSurface(background);
	for(int i=0; i < NUM_BULLETS; i++)
		SDL_FreeSurface(bulletSurface[i]);
	for(int i=0; i < NUM_BAD; i++)
		SDL_FreeSurface(bad[i]);
	TTF_CloseFont(font);
	SDL_FreeSurface(message);
	SDL_FreeSurface(titleScreen);

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
	//The timer starting time
    Uint32 start = SDL_GetTicks();
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

	//Set the wall
    wall[0].x = 0;
    wall[0].y = 0;
    wall[0].w = 0;//60;
    wall[0].h = 0;//500;

	wall[1].x = 0;
    wall[1].y = 0;
    wall[1].w = 640;
    wall[1].h = 50;
		
	/*wall[2].x = 636;
    wall[2].y = 60;
    wall[2].w = 4;
    wall[2].h = 490;
	
	wall[3].x = 60;
    wall[3].w = 580;
    wall[3].h = 4;*/

	int bgX = 0;
	int bgY = 0;
	bool scrollBgY = false;
	bool scrollBgX = false;

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the player, if not dead
			if(myPlayer.returnHp() > 0)
				myPlayer.handle_input();

			//If the user has pressed ESC
			if(event.type == SDL_KEYUP)
				if(event.key.keysym.sym == SDLK_ESCAPE)
					quit = true;

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
		}

		// Don't alow anything to move if player is dead
		if(myPlayer.returnHp() > 0)
		{
			//Move the player
			myPlayer.move();
			//Move bullets
			for(int i=0; i < NUM_BULLETS; i++)
				bullets[i].move();
			//Move enemies
			for(int i=0; i < NUM_BAD; i++)
				badguy[i].move();
		}
        //Fill the screen white
        //SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		//Show the background
        apply_surface(0,0, background, screen );

		//Show the wall
		for (int i = 0; i < 2; i++)
		{
			SDL_FillRect( screen, &wall[i], SDL_MapRGB( screen->format, 0x77, 0x77, 0x77 ) );
		}
		
        //Show the objects on the screen, if player is not dead
		if(myPlayer.returnHp() > 0)
		{
			myPlayer.show();
			for(int i=0; i < NUM_BULLETS; i++)
				bullets[i].show();
			for(int i=0; i < NUM_BAD; i++)
				badguy[i].show();
		}

		// Re-spawn all the enemies
		if(NUM_BAD_KILLED == NUM_BAD)
		{
			//// Delay three seconds
			//Uint32 badDelayStart = SDL_GetTicks();
			//// Loop through and respawn them
			//if((badDelayStart - SDL_GetTicks()) >= (1000*3))
			//{
				for(int i = 0; i < NUM_BAD; i++)
				{
					badguy[i].respawn(i);
				}
			//}
			NUM_BAD_KILLED = 0;
		}

		//Render the text
		sprintf(debug,"HP:%d ",myPlayer.returnHp());
		int tics = (1000 / FRAMES_PER_SECOND) - fps.get_ticks();
		sprintf(debug + strlen(debug),"FPS:%d ",tics);
		sprintf(debug + strlen(debug),"Time:%d", (SDL_GetTicks() - start)/1000);
		//sprintf(debug + strlen(debug)," No. killed:%d",NUM_BAD_KILLED);
		message = TTF_RenderText_Solid( font, debug, textColor );

		//If there was an error in rendering the text
		if( message == NULL )
			return 1;

		// Player dead, show game over text
		if(myPlayer.returnHp() <= 0)
		{
			SDL_Color redText = {255,0,0};
			message = TTF_RenderText_Solid(font,"Game Over",redText);
			apply_surface(240, 5, message, screen );
		}
		// Player alive, show HUD
		else
			apply_surface(5, 5, message, screen );

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    //Clean up
    clean_up();

    return 0;
}
