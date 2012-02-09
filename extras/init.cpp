#include <SDL/SDL.h>
#include <fstream>
#include <string>
#include "tools.hxx"



bool init(SDL_Surface* screen, player &TheOne, tile lol[], int layout[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 
			SDL_SWSURFACE);

	if(screen == NULL)
	{
		return false;
	}

	SDL_WM_SetCaption("TheEngine", NULL);

	std::string defSave = "savedef";

	if(!loadSave(defSave,TheOne, lol, layout))
	{
		return false;
	}

	TheOne.texture=load_image("player.bmp");
	TheOne.camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};	
}
