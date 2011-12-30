#ifndef INIT_H
#define INIT_H

#include <SDL\SDL.h>
#include "misc.hxx"



bool init(player &TheOne, map &world)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	std::string defSave = "save\\savedef";

	if(!loadSave(defSave, TheOne, world))
	{
		world.name = "data\\maps\\defMap1.mapx";
		TheOne.x = 0;
		TheOne.y = 0;
		if(!createSave(defSave, world.name, TheOne))
		{
			return false;
		}
	
		if(!loadSave(defSave, TheOne, world))
		{
			return false;
		}
	}

	TheOne.texture = load_image("data\\player.bmp");
	if(TheOne.texture == NULL)
	{
		return false;
	}

	return true;
}



void clean_up(window &mainWindow, map &world, player &TheOne)
{
	SDL_FreeSurface(mainWindow.screen);
	SDL_FreeSurface(TheOne.texture);

	for(int i=0; i< 50; i++)
	{
		SDL_FreeSurface(world.texture[i]);
	}

	SDL_Quit();
}

#endif
