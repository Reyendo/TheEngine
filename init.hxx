#ifndef INIT_H
#define INIT_H

#include "os.hxx"
#include "base-classes.hxx"
#include "tools.hxx"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>



bool init(window &mainWindow, player &TheOne, map &world)
{
	TTF_Font *coreFont;
	SDL_Event event;
	std::string userIn;
	std::string defSave = "save/savedef";
	std::string defMap = "data/maps/defMap1.mapx";
	timer fps;
	Uint32 windowColour = SDL_MapRGB(mainWindow.screen->format,255,255,255);

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	if(TTF_Init() == -1)
	{
		cout<< "TTF_Init() failed"<< endl;

		return false;
	}
	coreFont = load_font("fonts/ttf-bitstream-vera-1.10/VeraMoBd.ttf", 16);


	// DISPLAY MENU SCREEN
	//
	int phase = 0;
	while(1)
	{
		fps.start();
		while(SDL_PollEvent(&event))
		{
			mainWindow.handle_events(event);
			if(event.type == SDL_QUIT)
			{
				return false;
			}
			switch(phase)
			{
				case 0:
					stringInput(event, userIn, 20);
					if(event.key.keysym.sym == SDLK_RETURN)
					{
						if(userIn != "")
						{
							TheOne.name = userIn;
							userIn.clear();
							phase++;
						}
					}
					break;
				default:
					break;
			}
		}

		Uint8 *keystates = SDL_GetKeyState(NULL);
		if(keystates[SDLK_q])
		{
			return false;
		}

		SDL_FillRect(mainWindow.screen, NULL, windowColour);
		switch(phase)
		{
			case 0:
				SDL_Surface *text1 = drawtext(coreFont,1,1,1,1,0,0,0,1,
						"Character Name:", blended);
				SDL_Surface *text2 = drawtext(coreFont,1,1,1,1,0,0,0,1,
						userIn.c_str(), solid);
				break;
			default:
				break;
		}
		apply_surface(64, (SCREENHEIGHT/2)-64, text1, mainWindow.screen);
		apply_surface(text1->w+70, (SCREENHEIGHT/2)-64, text2, mainWindow.screen);
		SDL_Flip(mainWindow.screen);

		if(fps.get_ticks() < 1000/30)
		{
			SDL_Delay((1000/30) - fps.get_ticks());
		}
		if(phase > 1)
		{break;}
	}


	if(!loadSave(defSave, TheOne, world))
	{
		world.name = defMap;
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

	// TheOne.texture = load_image("data/player.bmp");
	TheOne.texture = load_image("data/player.bmp");
	TheOne.texture1 = load_image("data/player.bmp");
	TheOne.texture2 = load_image("data/player.bmp");
	TheOne.texture3 = load_image("data/player.bmp");
	TheOne.texture4 = load_image("data/player.bmp");

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
	SDL_FreeSurface(TheOne.texture1);
	SDL_FreeSurface(TheOne.texture2);
	SDL_FreeSurface(TheOne.texture3);
	SDL_FreeSurface(TheOne.texture4);

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

	for(unsigned int i=0;i<world.creatureList.size();i++)
	{
		SDL_FreeSurface(world.creatureList[i].texture);
		SDL_FreeSurface(world.creatureList[i].texture1);
		SDL_FreeSurface(world.creatureList[i].texture2);
		SDL_FreeSurface(world.creatureList[i].texture3);
		SDL_FreeSurface(world.creatureList[i].texture4);		
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
