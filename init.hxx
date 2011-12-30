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

	for(unsigned int i=0;i<TheOne.inventory.size();i++)
	{
		SDL_FreeSurface(TheOne.inventory[i]->texture);
		SDL_FreeSurface(TheOne.inventory[i]->texture1);
		SDL_FreeSurface(TheOne.inventory[i]->texture2);
		SDL_FreeSurface(TheOne.inventory[i]->texture3);
		SDL_FreeSurface(TheOne.inventory[i]->texture4);		
	}

	for(unsigned int i=0;i<TheOne.quiver.size();i++)
	{
		SDL_FreeSurface(TheOne.quiver[i].texture);
		SDL_FreeSurface(TheOne.quiver[i].texture1);
		SDL_FreeSurface(TheOne.quiver[i].texture2);
		SDL_FreeSurface(TheOne.quiver[i].texture3);
		SDL_FreeSurface(TheOne.quiver[i].texture4);		
	}

	for(int i=0; i< 50; i++)
	{
		SDL_FreeSurface(world.texture[i]);
	}

	for(int i=0; i<MAPSIZE; i++)
	{
		SDL_FreeSurface(world.tileList[i].texture);
	}

	for(unsigned int i=0;i<world.creatureList.size();i++)
	{
		SDL_FreeSurface(world.creatureList[i].texture);
	}

	for(unsigned int i=0;i<world.projectiles.size();i++)
	{
		SDL_FreeSurface(world.projectiles[i].texture);
		SDL_FreeSurface(world.projectiles[i].texture1);
		SDL_FreeSurface(world.projectiles[i].texture2);
		SDL_FreeSurface(world.projectiles[i].texture3);
		SDL_FreeSurface(world.projectiles[i].texture4);		
	}

	SDL_Quit();
}

#endif
