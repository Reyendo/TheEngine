#ifndef INIT_H
#define INIT_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "base-classes.hxx"
#include "os.hxx"
#include "tools.hxx"



bool init(window &mainWindow, player &TheOne, map &world)
{
	TTF_Font *coreFont, *headerFont;
	SDL_Event event;
	SDL_Surface *text1, *text2, *text3, *text4, *text5,*outline;
	std::string userIn;
	//std::string defMap = "defMap1";
	std::string defMap = "testMap";
	timer fps;
	Uint32 windowColour = SDL_MapRGB(mainWindow.screen->format,255,255,255);
	bool quit = false;

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	if(TTF_Init() == -1)
	{
		cout<< "TTF_Init() failed"<< endl;

		return false;
	}
	coreFont = load_font("../data/fonts/ttf-bitstream-vera-1.10/VeraMoBd.ttf", 
			16);
	headerFont = load_font("../data/fonts/ttf-bitstream-vera-1.10/VeraMoBd.ttf",
			24);


	// DISPLAY MENU SCREEN
	//
	int phase = 0;
	int iterator = 0;
	text1 = drawtext(headerFont,1,1,1,1,0,0,0,1,
			"New Save", textType::blended);
	text2 = drawtext(headerFont,1,1,1,1,0,0,0,1,
			"Load Save", textType::blended);
	text3 = drawtext(coreFont,1,1,1,1,0,0,0,1,
			"Character Name:", textType::blended);
	text4 = drawtext(coreFont,1,1,1,1,0,0,0,1,
			"Greetings, ", textType::blended);
	outline = load_image("../data/sprites/outline2.bmp");
	if(outline == NULL)
	{
		cout<< "Error: outline was not loaded"<< endl;
	}


	if(SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL) != 0)
	{cout<< "SDL_EnableKeyRepeat failed."<< endl;}

	while(!quit)
	{
		fps.start();
		while(SDL_PollEvent(&event))
		{
			mainWindow.handle_events(event);
			if(event.type == SDL_QUIT)
			{
				return false;
			}

			SDL_FillRect(mainWindow.screen,NULL,windowColour);
			switch(phase)
			{
				case 0: 
					if(event.key.keysym.sym == SDLK_UP)
					{
						if(iterator > 0)
						{iterator--;}
					}else if(event.key.keysym.sym == SDLK_DOWN)
					{
						if(iterator < 1)
						{iterator++;}
					}else if(event.key.keysym.sym == SDLK_RETURN)
					{
						if(iterator == 0)
						{
							phase = 1;
						}else if(iterator == 1)
						{
							phase = 2;
						}
					}
					apply_surface((SCREENWIDTH/2)-(text1->w/2),(SCREENHEIGHT/2)-30,
							text1,mainWindow.screen);
					apply_surface((SCREENWIDTH/2)-(text2->w/2),(SCREENHEIGHT/2)+30,
							text2,mainWindow.screen);
					if(iterator == 0)
					{
						apply_surface((SCREENWIDTH/2)-(outline->w/2),
								(SCREENHEIGHT/2)-30,outline,mainWindow.screen);
					}else if(iterator == 1)
					{
						apply_surface((SCREENWIDTH/2)-(outline->w/2),
								(SCREENHEIGHT/2)+30,outline,mainWindow.screen);
					}
					break;
				case 1:
					if(event.key.keysym.sym==SDLK_RETURN && userIn.size()>0)
					{
						TheOne.name = userIn;
						world.name = defMap;
						TheOne.x = 0;
						TheOne.y = 0;
						if(!createSave(TheOne.name, world, TheOne))
						{
							return false;
						}

						if(!loadSave(TheOne.name, world, TheOne))
						{
							return false;
						}
						userIn.clear();
						phase = 2;
					}else
					{
						stringInput(event,userIn,20);
					}
					text5 = drawtext(coreFont,1,1,1,1,0,0,0,1,
							userIn.c_str(),textType::solid);
					apply_surface(64,(SCREENHEIGHT/2)-64,text3,
							mainWindow.screen);
					apply_surface(text3->w+70,(SCREENHEIGHT/2)-64,
							text5,mainWindow.screen);
					break;
				case 2:
					if(event.key.keysym.sym = SDLK_RETURN)
					{
						phase = 3;
					}
					text5 = drawtext(coreFont,1,1,1,1,0,0,0,1,
							TheOne.name.c_str(),textType::solid);
					apply_surface((SCREENWIDTH/2)-((text4->w+text5->w)/2),
							(SCREENHEIGHT/2)-64,text4,mainWindow.screen);
					apply_surface((SCREENWIDTH/2)-((text4->w+text5->w)/2)+
							text4->w,(SCREENHEIGHT/2)-64,text5,
							mainWindow.screen);
					break;
				default:
					quit = true;
					break;
			}
		}
		SDL_Flip(mainWindow.screen);

		if(fps.get_ticks() < 1000/5)
		{
			SDL_Delay((1000/5) - fps.get_ticks());
		}
	}


		/*
		fps.start();
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				return false;
			}
			switch(phase)
			{
				case 0:
					if(event.key.keysym.sym == SDLK_UP)
					{
						if(iterator > 0)
							iterator--;
					}else if(event.key.keysym.sym == SDLK_DOWN)
					{
						if(iterator < 1)
							iterator++;
					}else if(event.key.keysym.sym == SDLK_RETURN)
					{
						if(iterator == 0)
						{
							phase = 1;
						}else if(iterator == 1)
						{
							phase = 2;
						}
					}
					break;
				case 1:
					if(event.key.keysym.sym==SDLK_RETURN && userIn.size()>0) 
					{
						TheOne.name = userIn;
						world.name = defMap;
						TheOne.x = 0;
						TheOne.y = 0;
						if(!createSave(TheOne.name, world, TheOne))
						{
							return false;
						}

						if(!loadSave(TheOne.name, world, TheOne))
						{
							return false;
						}
						userIn.clear();
						phase = 2;
					}else
					{
						stringInput(event,userIn,20);
					}
					break;
				case 2:
					if(event.key.keysym.sym == SDLK_RETURN)
					{
						phase = 3;
					}
					break;
				default:
					quit = true;
					break;
			}
			mainWindow.handle_events(event);
		}
		
		//RENDERING
		//
		SDL_FillRect(mainWindow.screen,NULL,windowColour);
		switch(phase)
		{
			case 0:
				apply_surface((SCREENWIDTH/2)-(text1->w/2),(SCREENHEIGHT/2)-30,
						text1,mainWindow.screen);
				apply_surface((SCREENWIDTH/2)-(text2->w/2),(SCREENHEIGHT/2)+30,
						text2,mainWindow.screen);
				break;
			case 1:
				text5 = drawtext(coreFont,1,1,1,1,0,0,0,1,
						userIn.c_str(), solid);
				apply_surface(64, (SCREENHEIGHT/2)-64,text3,
						mainWindow.screen);
				apply_surface(text3->w+70, (SCREENHEIGHT/2)-64,
						text5, mainWindow.screen);
				break;
			case 2:
				break;
			case 3:
				text5 = drawtext(coreFont,1,1,1,1,0,0,0,1,
						TheOne.name.c_str(), solid);
				apply_surface(54, (SCREENHEIGHT/2)-(text4->h/2),text4,
						mainWindow.screen);
				apply_surface(text4->w+70, (SCREENHEIGHT/2)-(text5->h/2),
						text5, mainWindow.screen);
				break;
			default:
				break;
		}
		SDL_Flip(mainWindow.screen);

		if(fps.get_ticks() < 1000/5)
		{
			SDL_Delay((1000/5) - fps.get_ticks());
		}
	}
	*/

	if(SDL_EnableKeyRepeat(0,0) != 0)
	{cout<< "SDL_EnableKeyRepeat failed."<< endl;}

	for(int i=0;i<5;i++)
	{
		TheOne.texture[i] = load_image("../data/sprites/player.bmp");
	}

	if(TheOne.texture == NULL)
	{
		return false;
	}

	return true;
}



void clean_up(window &mainWindow, map &world, player &TheOne)
{
	SDL_FreeSurface(mainWindow.screen);
	for(int i=0;i<5;i++)
	{
		SDL_FreeSurface(TheOne.texture[i]);
	}

	for(unsigned int i=0;i<TheOne.inventory.size();i++)
	{
		for(int j=0;j<5;j++)
		{
			SDL_FreeSurface(TheOne.inventory[i]->texture[j]);
		}
	}

	for(unsigned int i=0;i<TheOne.quiver.size();i++)
	{
		for(int j=0;j<5;j++)
		{
			SDL_FreeSurface(TheOne.quiver[i].texture[j]);
		}
	}

	for(int i=0; i< 50; i++)
	{
		SDL_FreeSurface(world.texture[i]);
	}

	for(unsigned int i=0;i<world.creatureList.size();i++)
	{
		for(int j=0;j<5;j++)
		{
			SDL_FreeSurface(world.creatureList[i].texture[j]);
		}
	}

	for(unsigned int i=0;i<world.projectiles.size();i++)
	{
		for(int j=0;j<5;j++)
		{
			SDL_FreeSurface(world.projectiles[i].texture[j]);
		}
	}

	SDL_Quit();
}

#endif
