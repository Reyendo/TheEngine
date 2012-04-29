#include "base-classes.hxx"


#include "tools.hxx"
#include "os.hxx"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <stdlib.h>
#ifdef WINDOWS
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <windows.h>
#endif
#ifdef LINUX 
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif




// CONSTRUCTORS

thing::thing()
	: x(0)
	, y(0)
	, w(SPRITEWIDTH)
	, h(SPRITEHEIGHT)
	, xVel(0)
	, yVel(0)
{}


container::container()
{}


tile::tile()
	: type(0)
	, texture(NULL)
{}


item::item()
	: texture(NULL)
	, texture1(NULL)
	, texture2(NULL)
	, texture3(NULL)
	, texture4(NULL)
	, type(UNASSIGNED)
{}


weapon::weapon()
	: damage(0)
	, effect(0)
	, range(0)
	, ranged(false)
{type = WEAPON;}


ammo::ammo()
	: direction(0)
	, damage(0)
	, effect(0)
{}


creature::creature()
	: race(0)
	, hp(0)
	, mp(0)
	, status(0)
	, strength(0)
	, endurance(0)
	, agility(0)
	, intelligence(0)
	, charisma(0)
	, wisdom(0)
	, will(0)
	, texture(NULL)
	, texture1(NULL)
	, texture2(NULL)
	, texture3(NULL)
	, texture4(NULL)
	, direction(0)
{
	camera.x=0;
	camera.y=0;
	camera.w=SCREENWIDTH;
	camera.h=SCREENHEIGHT;
}


player::player()
	: weapon_drawn(false)
	, inventoryFlag(false)
	, interactFlag(false)
	, keysense(0)
{}


npc::npc()
{}


enemy::enemy()
{}


map::map()
{}


window::window()
{
	screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, SCREENBPP,
			SDL_SWSURFACE | SDL_RESIZABLE);

	if(screen == NULL)
	{
		windowOK = false;
		return;
	}else
	{
		windowOK = true;
	}

	SDL_WM_SetCaption("The Engine", NULL);

	windowed = true;
}


timer::timer()
	: startTicks(0)
	, pausedTicks(0)
	, paused(false)
	, started(false)
{}




// BODY FUNCTIONS


// creature
//
void creature::show(SDL_Surface *&screen)
{
	if(x>camera.x&&x+w<camera.x+camera.w&&y>camera.y&&y+h<camera.y+camera.h)
	{	
		apply_surface(x-camera.x, y-camera.y, texture, screen);
	}
}



// player
//
bool player::handleInput(Uint8 *keystates)
{
	if(keystates[SDLK_RIGHT])
	{
		xVel = PLAYERVELOCITY;
	}else if(keystates[SDLK_LEFT])
	{
		xVel = -PLAYERVELOCITY;
	}else
	{
		xVel = 0;
	}

	if(keystates[SDLK_UP])
	{
		yVel = -PLAYERVELOCITY;
	}else if(keystates[SDLK_DOWN])
	{
		yVel = PLAYERVELOCITY;
	}else
	{
		yVel = 0;
	}

	if(keystates[SDLK_SPACE])
	{
		if(keysense > 10)
		{
			weapon_drawn = true;
			keysense = 0;
		}
		keysense++;
	}

	if(keystates[SDLK_i])
	{
		inventoryFlag=true;
	}

	if(keystates[SDLK_e])
	{
		interactFlag=true;
	}

	return true;
}


bool player::move(map &map1, Uint32 deltaTicks)
{
	
	if(xVel == 0 && yVel == 0)
	{
		return false;
	}

	x += xVel * (deltaTicks/1000.f);
	y += yVel * (deltaTicks/1000.f);

	if(x < 0)
	{
		x = 0;
	}else if(x+SPRITEWIDTH > MAPWIDTH*TILEWIDTH)
	{
		x = (MAPWIDTH*TILEWIDTH) - SPRITEWIDTH;
	}

	if(y < 0)
	{
		y = 0;
	}else if(y+TILEHEIGHT > MAPHEIGHT*TILEHEIGHT)
	{
		y = (MAPHEIGHT*TILEHEIGHT) - SPRITEHEIGHT;
	}
	
	int lol;
	for(int i=0;i<MAPSIZE;i++)
	{
		lol = collision(*this, map1.tileList[i]);
		if(lol)
		{
			/*
			switch(map1.tileList[i].type)
			{
				case 5:
					if(lol == 1)
					{x=map1.tileList[i].x-SPRITEWIDTH;}
					else if(lol == 2)
					{x=map1.tileList[i].x+map1.tileList[i].w;}
					else if(lol == 3)
					{y=map1.tileList[i].y-SPRITEWIDTH;}
					else
					{y=map1.tileList[i].y+map1.tileList[i].h;}
					break;
				case 6:
					map1.name = map1.tileList[i].data;
					map1.readMap();
					map1.loadStage();
					if(lol == 1)
					{x = 0;}
					else if(lol == 2)
					{x = MAPWIDTH-SPRITEWIDTH;}
					else if(lol == 3)
					{y = 0;}
					else if(lol == 4)
					{y = MAPHEIGHT - SPRITEHEIGHT;}
					break;
			}
			*/


			if(getFlag(map1.tileList[i].type, solid))
			{
				if(lol == 1)
				{x=map1.tileList[i].x-SPRITEWIDTH;}
				else if(lol == 2)
				{x=map1.tileList[i].x+map1.tileList[i].w;}
				else if(lol == 3)
				{y=map1.tileList[i].y-SPRITEWIDTH;}
				else
				{y=map1.tileList[i].y+map1.tileList[i].h;}
			}
		}
	}

	for(unsigned int i=0; i<map1.creatureList.size();i++)
	{
		lol = collision(*this, map1.creatureList[i]);
		switch(lol)
		{
			case 1:
				x=map1.creatureList[i].x-SPRITEWIDTH;
				break;
			case 2:
				x=map1.creatureList[i].x+map1.creatureList[i].w;
				break;
			case 3:
				y=map1.creatureList[i].y-SPRITEWIDTH;
				break;
			case 4:
				y=map1.creatureList[i].y+map1.creatureList[i].h;
				break;
			default:
				break;
		}
	}

	if(xVel > 0)
	{
		direction = 2;
	}else if(xVel < 0)
	{
		direction = 4;
	}
	if(yVel > 0)
	{
		direction = 1;
	}else if(yVel < 0)
	{
		direction = 3;
	}

	return true;
}


// REVAMP LATER
void player::draw_weapon(map &map1, window &mainWindow)
{
	if(!primaryWeapon.ranged)
	{
		switch(direction)
		{
			case 1:
				apply_surface(x-camera.x, (y+h)-camera.y, 
						primaryWeapon.texture1, mainWindow.screen);
				for(unsigned int i=0; i<map1.creatureList.size(); i++)
				{
					if(y+h+primaryWeapon.range>map1.creatureList[i].y&&y+h+primaryWeapon.range<map1.creatureList[i].y+map1.creatureList[i].h&&((x>map1.creatureList[i].x&&x<map1.creatureList[i].x+map1.creatureList[i].w)||(x+w<map1.creatureList[i].x+map1.creatureList[i].w&&x+w>map1.creatureList[i].x)||(x==map1.creatureList[i].x||x+w==map1.creatureList[i].x+map1.creatureList[i].w)))
					{
						map1.creatureList[i].hp-=primaryWeapon.damage;
					}
				}
				break;
			case 2:
				apply_surface((x+w)-camera.x, y-camera.y,
					   primaryWeapon.texture2, mainWindow.screen);
				for(unsigned int i=0; i<map1.creatureList.size(); i++)
				{
					if(x+w+primaryWeapon.range>map1.creatureList[i].x&&x+w+primaryWeapon.range<map1.creatureList[i].x+map1.creatureList[i].w&&((y>map1.creatureList[i].y&&y<map1.creatureList[i].y+map1.creatureList[i].h)||(y+h<map1.creatureList[i].y+map1.creatureList[i].h&&y+h>map1.creatureList[i].y)||(y==map1.creatureList[i].y||y+h==map1.creatureList[i].y+map1.creatureList[i].h)))
					{
						map1.creatureList[i].hp-= primaryWeapon.damage;
					}
				}
				break;
			case 3:
				apply_surface(x-camera.x, (y-h)-camera.y,
						primaryWeapon.texture3, mainWindow.screen);
				for(unsigned int i=0; i<map1.creatureList.size(); i++)
				{
					if(y-primaryWeapon.range<map1.creatureList[i].y+map1.creatureList[i].h&&y-primaryWeapon.range>map1.creatureList[i].y&&((x>map1.creatureList[i].x&&x<map1.creatureList[i].x+map1.creatureList[i].w)||(x+w<map1.creatureList[i].x+map1.creatureList[i].w&&x+w>map1.creatureList[i].x)||(x==map1.creatureList[i].x||x+w==map1.creatureList[i].x+map1.creatureList[i].w)))
					{
						map1.creatureList[i].hp-=primaryWeapon.damage;
					}
				}
				break;
			case 4:
				apply_surface((x-w)-camera.x, y-camera.y,
						primaryWeapon.texture4, mainWindow.screen);
				for(unsigned int i=0; i<map1.creatureList.size(); i++)
				{
					if(x-primaryWeapon.range<map1.creatureList[i].x+map1.creatureList[i].w&&x-primaryWeapon.range>map1.creatureList[i].x&&((y>map1.creatureList[i].y&&y<map1.creatureList[i].y+map1.creatureList[i].h)||(y+h<map1.creatureList[i].y+map1.creatureList[i].h&&y+h>map1.creatureList[i].y)||(y==map1.creatureList[i].y||y+h==map1.creatureList[i].y+map1.creatureList[i].h)))
					{
						map1.creatureList[i].hp-=primaryWeapon.damage;
					}
				}
				break;
			default:
				break;
		}
	}else
	{
		if(quiver.size()>0){
			switch(direction)
			{
				case 1:
					quiver.back().x = x;
					quiver.back().y = y+h;
					quiver.back().yVel = PLAYERVELOCITY*2;
					map1.projectiles.push_back(quiver.back());
					quiver.pop_back();
					break;
				case 2:
					quiver.back().x = x+w;
					quiver.back().y = y;
					quiver.back().xVel = PLAYERVELOCITY*2;
					map1.projectiles.push_back(quiver.back());
					quiver.pop_back();
					break;
				case 3:
					quiver.back().x = x;
					quiver.back().y = y-quiver.back().h;
					quiver.back().yVel = -PLAYERVELOCITY*2;
					map1.projectiles.push_back(quiver.back());
					quiver.pop_back();
					break;
				case 4:
					quiver.back().x = x-quiver.back().w;
					quiver.back().y = y;
					quiver.back().xVel = -PLAYERVELOCITY*2;
					map1.projectiles.push_back(quiver.back());
					quiver.pop_back();
					break;
				default:
					break;
			}
		}
	}
}


void player::checkInventory(window &mainWindow)
{
	int j = 0;
	inventoryFlag = false;
	SDL_Event event;
	tile select;
	select.x = 0;
	select.y = 0;
	select.w = 32;
	select.h = 32;
	select.texture = load_image(filePath("data/outline.bmp"));
	timer fps;
	Uint32 windowColour = SDL_MapRGB(mainWindow.screen->format, 255, 255, 255);

	bool quit(false);
	while(!quit)
	{
		fps.start();
		while(SDL_PollEvent(&event))
		{
			mainWindow.handle_events(event);
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		Uint8 *keystates = SDL_GetKeyState(NULL);
		if(keystates[SDLK_q])
		{quit=true;}
		else if(keystates[SDLK_UP]) // FIX CODE HERE; ADD J MODIFIER
		{
			select.y -= SPRITEHEIGHT*2;
			if(select.y<0){select.y+=SPRITEHEIGHT*2;}
		}else if(keystates[SDLK_DOWN]) // HERE TOO
		{
			select.y += SPRITEHEIGHT*2;
			if(select.y+select.h>SCREENHEIGHT){select.y-=SPRITEHEIGHT*2;}
		}else if(keystates[SDLK_RIGHT])
		{
			j += 1;
			select.x += SPRITEWIDTH*2;
			if(select.x+select.w>SCREENWIDTH){select.x-=SPRITEWIDTH*2; j-=1;}
		}else if(keystates[SDLK_LEFT])
		{
			j -= 1;
			select.x -= SPRITEWIDTH*2;
			if(select.x<0){select.x+=SPRITEWIDTH*2; j+=1;}
		}else if(keystates[SDLK_SPACE])
		{
			if(inventory[j]->type == WEAPON)
			{
				primaryWeapon = *reinterpret_cast<weapon*>(inventory[j]);
			}
		}

		SDL_FillRect(mainWindow.screen, NULL, windowColour);
		int itemX(0), itemY(0);
		for(unsigned int i=0;i<inventory.size();i++)
		{
			apply_surface(itemX, itemY, inventory[i]->texture,
				   	mainWindow.screen);
			if(itemX+inventory[i]->w+SPRITEWIDTH > SCREENWIDTH)
			{itemY += inventory[i]->w + SPRITEWIDTH; itemX = 0;}
			else{itemX += inventory[i]->w+SPRITEWIDTH;}
		}
		apply_surface(select.x, select.y, select.texture, mainWindow.screen);
		SDL_Flip(mainWindow.screen);

		if(fps.get_ticks() < 1000/12)
		{
			SDL_Delay((1000/12) - fps.get_ticks());
		}	
	}
}


void player::interact(map &map1, window &mainWindow)
{
	switch(direction)
	{
		case 1:
			apply_surface(x-camera.x, (y+h)-camera.y, 
					primaryWeapon.texture1, mainWindow.screen);
			for(unsigned int i=0; i<MAPSIZE; i++)
			{
				if(y+h+PLAYERREACH>map1.tileList[i].y&&y+h+PLAYERREACH<map1.tileList[i].y+map1.tileList[i].h&&((x>map1.tileList[i].x&&x<map1.tileList[i].x+map1.tileList[i].w)||(x+w<map1.tileList[i].x+map1.tileList[i].w&&x+w>map1.tileList[i].x)||(x==map1.tileList[i].x||x+w==map1.tileList[i].x+map1.tileList[i].w)))
				{

				}
			}
			break;
		case 2:
			apply_surface((x+w)-camera.x, y-camera.y,
				   primaryWeapon.texture2, mainWindow.screen);
			for(unsigned int i=0; i<MAPSIZE; i++)
			{
				if(x+w+PLAYERREACH>map1.tileList[i].x&&x+w+PLAYERREACH<map1.tileList[i].x+map1.tileList[i].w&&((y>map1.tileList[i].y&&y<map1.tileList[i].y+map1.tileList[i].h)||(y+h<map1.tileList[i].y+map1.tileList[i].h&&y+h>map1.tileList[i].y)||(y==map1.tileList[i].y||y+h==map1.tileList[i].y+map1.tileList[i].h)))
				{
					// ADD STUFF LATER
				}
			}
			break;
		case 3:
			apply_surface(x-camera.x, (y-h)-camera.y,
					primaryWeapon.texture3, mainWindow.screen);
			for(unsigned int i=0; i<MAPSIZE; i++)
			{
				if(y-PLAYERREACH<map1.tileList[i].y+map1.tileList[i].h&&y-PLAYERREACH>map1.tileList[i].y&&((x>map1.tileList[i].x&&x<map1.tileList[i].x+map1.tileList[i].w)||(x+w<map1.tileList[i].x+map1.tileList[i].w&&x+w>map1.tileList[i].x)||(x==map1.tileList[i].x||x+w==map1.tileList[i].x+map1.tileList[i].w)))
				{
					// ADD STUFF LATER
				}
			}
			break;
		case 4:
			apply_surface((x-w)-camera.x, y-camera.y,
					primaryWeapon.texture4, mainWindow.screen);
			for(unsigned int i=0; i<MAPSIZE; i++)
			{
				if(x-PLAYERREACH<map1.tileList[i].x+map1.tileList[i].w&&x-PLAYERREACH>map1.tileList[i].x&&((y>map1.tileList[i].y&&y<map1.tileList[i].y+map1.tileList[i].h)||(y+h<map1.tileList[i].y+map1.tileList[i].h&&y+h>map1.tileList[i].y)||(y==map1.tileList[i].y||y+h==map1.tileList[i].y+map1.tileList[i].h)))
				{
					// ADD STUFF LATER
				}
			}
			break;
		default:
			break;
	}
		switch(direction)
	{
		case 1:
			apply_surface(x-camera.x, (y+h)-camera.y, 
					primaryWeapon.texture1, mainWindow.screen);
			for(unsigned int i=0; i<map1.creatureList.size(); i++)
			{
				if(y+h+PLAYERREACH>map1.creatureList[i].y&&y+h+PLAYERREACH<map1.creatureList[i].y+map1.creatureList[i].h&&((x>map1.creatureList[i].x&&x<map1.creatureList[i].x+map1.creatureList[i].w)||(x+w<map1.creatureList[i].x+map1.creatureList[i].w&&x+w>map1.creatureList[i].x)||(x==map1.creatureList[i].x||x+w==map1.creatureList[i].x+map1.creatureList[i].w)))
				{
					// ADD STUFF LATER
				}
			}
			break;
		case 2:
			apply_surface((x+w)-camera.x, y-camera.y,
				   primaryWeapon.texture2, mainWindow.screen);
			for(unsigned int i=0; i<map1.creatureList.size(); i++)
			{
				if(x+w+PLAYERREACH>map1.creatureList[i].x&&x+w+PLAYERREACH<map1.creatureList[i].x+map1.creatureList[i].w&&((y>map1.creatureList[i].y&&y<map1.creatureList[i].y+map1.creatureList[i].h)||(y+h<map1.creatureList[i].y+map1.creatureList[i].h&&y+h>map1.creatureList[i].y)||(y==map1.creatureList[i].y||y+h==map1.creatureList[i].y+map1.creatureList[i].h)))
				{
					// ADD STUFF LATER
				}
			}
			break;
		case 3:
			apply_surface(x-camera.x, (y-h)-camera.y,
					primaryWeapon.texture3, mainWindow.screen);
			for(unsigned int i=0; i<map1.creatureList.size(); i++)
			{
				if(y-PLAYERREACH<map1.creatureList[i].y+map1.creatureList[i].h&&y-PLAYERREACH>map1.creatureList[i].y&&((x>map1.creatureList[i].x&&x<map1.creatureList[i].x+map1.creatureList[i].w)||(x+w<map1.creatureList[i].x+map1.creatureList[i].w&&x+w>map1.creatureList[i].x)||(x==map1.creatureList[i].x||x+w==map1.creatureList[i].x+map1.creatureList[i].w)))
				{
					// ADD STUFF LATER
				}
			}
			break;
		case 4:
			apply_surface((x-w)-camera.x, y-camera.y,
					primaryWeapon.texture4, mainWindow.screen);
			for(unsigned int i=0; i<map1.creatureList.size(); i++)
			{
				if(x-PLAYERREACH<map1.creatureList[i].x+map1.creatureList[i].w&&x-PLAYERREACH>map1.creatureList[i].x&&((y>map1.creatureList[i].y&&y<map1.creatureList[i].y+map1.creatureList[i].h)||(y+h<map1.creatureList[i].y+map1.creatureList[i].h&&y+h>map1.creatureList[i].y)||(y==map1.creatureList[i].y||y+h==map1.creatureList[i].y+map1.creatureList[i].h)))
				{
					// ADD STUFF LATER
				}
			}
			break;
		default:
			break;
	}
}


void player::set_camera()
{
	camera.x = (x+SPRITEWIDTH/2) - SCREENWIDTH/2;
	camera.y = (y+SPRITEHEIGHT/2) - SCREENHEIGHT/2;

	if(camera.x < 0)
	{
		camera.x = 0;
	}

	if(camera.y < 0)
	{
		camera.y = 0;
	}

	if (camera.x+camera.w > (MAPWIDTH*TILEWIDTH))
	{
		camera.x = (MAPWIDTH*TILEWIDTH) - camera.w;
	}

	if(camera.y+camera.h > (MAPHEIGHT*TILEHEIGHT))
	{
		camera.y = (MAPHEIGHT*TILEHEIGHT) - camera.h;
	}
}


void player::show(SDL_Surface *&screen)
{
	apply_surface(x-camera.x, y-camera.y, texture, screen);
}


bool npc::think(player &TheOne)
{
	if(x<TheOne.x)
	{
		xVel = (PLAYERVELOCITY/4);
	}else if(x>TheOne.x)
	{
		xVel = -(PLAYERVELOCITY/4);
	}else
	{
		xVel = 0;
	}
	if(y<TheOne.y)
	{
		yVel = (PLAYERVELOCITY/4);
	}else if(y>TheOne.y)
	{
		yVel = -(PLAYERVELOCITY/4);
	}else
	{
		yVel = 0;
	}

	return true;
}



// npc
//
void npc::move(map &map1, player &TheOne, Uint32 deltaTicks)
{	
	if(xVel == 0 && yVel == 0)
	{
		return;
	}

	x += xVel * (deltaTicks/1000.f);
	y += yVel * (deltaTicks/1000.f);

	if(x < 0)
	{
		x = 0;
	}else if(x+SPRITEWIDTH > MAPWIDTH*TILEWIDTH)
	{
		x = (MAPWIDTH*TILEWIDTH) - SPRITEWIDTH;
	}

	if(y < 0)
	{
		y = 0;
	}else if(y+TILEHEIGHT > MAPHEIGHT*TILEHEIGHT)
	{
		y = (MAPHEIGHT*TILEHEIGHT) - SPRITEHEIGHT;
	}

	int lol;
	for(int i=0;i<MAPSIZE;i++)
	{
		lol = collision(*this, map1.tileList[i]);
		if(lol)
		{
			switch(map1.tileList[i].type)
			{
				case 5:
					if(lol == 1)
					{x=map1.tileList[i].x-SPRITEWIDTH;}
					else if(lol == 2)
					{x=map1.tileList[i].x+map1.tileList[i].w;}
					else if(lol == 3)
					{y=map1.tileList[i].y-SPRITEWIDTH;}
					else
					{y=map1.tileList[i].y+map1.tileList[i].h;}
			}
		}
	}

	for(unsigned int i=0; i<map1.creatureList.size();i++)
	{
		lol = collision(*this, map1.creatureList[i]);
		switch(lol)
		{
			case 1:
				x=map1.creatureList[i].x-SPRITEWIDTH;
				break;
			case 2:
				x=map1.creatureList[i].x+map1.creatureList[i].w;
				break;
			case 3:
				y=map1.creatureList[i].y-SPRITEWIDTH;
				break;
			case 4:
				y=map1.creatureList[i].y+map1.creatureList[i].h;
				break;
			default:
				break;
		}
	}

	switch(collision(*this, TheOne))
	{
		case 1:
			x=TheOne.x-w;
			break;
		case 2:
			x=TheOne.x+TheOne.w;
			break;
		case  3:
			y=TheOne.y-h;
			break;
		case 4:
			y=TheOne.y+TheOne.h;
			break;
		default:
			break;
	}

	if(xVel > 0)
	{
		direction = 2;
	}else if(xVel < 0)
	{
		direction = 4;
	}
	if(yVel > 0)
	{
		direction = 1;
	}else if(yVel < 0)
	{
		direction = 3;
	}

}



// map
//
bool map::readMap()
{
	ifstream map;
	map.open(name.c_str());
	if(map.is_open())
	{
		creatureList.clear();

		int i(0);
		string line, token;
		while(getline(map, line))
		{
			if(i < 50)
			{
				for(unsigned int j=0; j<line.length(); j++)
				{
					if(line[j] == ',')
					{
						texName[i] = token;
						token.erase(0, token.length());
						i++;
					}else{token+=line[j];}
				}
			}else if(i<100)
			{
				for(unsigned int j=0; j<line.length(); j++)
				{
					if(line[j] == ',')
					{
						tileType[i-50] = atoi(token.c_str());
						token.erase(0, token.length());
						i++;
					}else{token+=line[j];}
				}
			}else if(i<150)
			{
				for(unsigned int j=0; j<line.length(); j++)
				{
					if(line[j] == ',')
					{
						tileData[i-100] = token;
						token.erase(0, token.length());
						i++;
					}else{token+=line[j];}
				}
			}else if(i<MAPSIZE+150)
			{
				for(unsigned int j=0;j<line.length();j++)
				{
					if(line[j]==',')
					{
						layout[i-150]=atoi(token.c_str());
						token.erase(0, token.length());
						i++;
					}else{token+=line[j];}
				}
			}else
			{
				for(unsigned int j=0;j<line.length();j++)
				{
					if(line[j]==',')
					{
						npc mapNPC;
						int l(0);
						std::string temp;
						for(unsigned int k=0;k<j;k++)
						{
							if(token[k]=='%')
							{
								if(l==0)
								{
									mapNPC.name=temp;
								}else if(l==1)
								{
									mapNPC.x=atoi(temp.c_str());
								}else if(l==2)
								{
									mapNPC.y=atoi(temp.c_str());
								}else if(l==3)
								{
									mapNPC.w=atoi(temp.c_str());
								}else if(l==4)
								{
									mapNPC.h=atoi(temp.c_str());
								}
								temp.erase(0,temp.length());
								l++;
							}else{temp+=token[k];}
						}
						mapNPC.hp=atoi(temp.c_str());
						creatureList.push_back(mapNPC);
						i++;
					}else{token+=line[j];}
				}
			}
		}

		map.close();
		return true;
	}else
	{
		return false;
	}
}


bool map::loadStage()
{
	for(int i=0; i<50; i++)
	{
		if(texName[i] != "" || "0")
		{
			#ifdef WINDOWS
			texture[i] = load_image("data\\"+texName[i]);
			#endif
			#ifdef LINUX
			texture[i] = load_image("data/"+texName[i]);
			#endif
		}else
		{
			break;
		}
	}

	int j = 0;
	for(int y=0; y<MAPWIDTH; y++)
	{
		for(int x=0; x<MAPHEIGHT; x++)
		{
			tileList[j].x = x*TILEWIDTH;
			tileList[j].y = y*TILEHEIGHT;
			tileList[j].w = TILEWIDTH;
			tileList[j].h = TILEHEIGHT;
			tileList[j].type = tileType[layout[j]];
			#ifdef WINDOWS
			tileList[j].link = "data\\maps\\"+tileLink[layout[j]];
			#endif
			#ifdef LINUX
			tileList[j].data = "data/maps/"+tileData[layout[j]];
			#endif
			tileList[j].texture = texture[layout[j]];
			j++;
		}
	}

	for(unsigned int i=0;i<creatureList.size();i++)
	{
		#ifdef WINDOWS
		creatureList[i].texture=load_image("data\\"+creatureList[i].name);
		#endif
		#ifdef LINUX
		creatureList[i].texture=load_image("data/"+creatureList[i].name);
		if(creatureList[i].texture == NULL)
		{
			cout<< "load_image error."<< endl;
		}
		#endif
	}

	return true;
}


void map::projectileLoop(player &TheOne, Uint32 deltaTicks)
{
	vector<ammo>::iterator it = projectiles.begin();
	for(unsigned int j=0;j<projectiles.size();j++)
	{
		if(projectiles[j].xVel == 0 && projectiles[j].yVel == 0)
		{
			return;
		}

		projectiles[j].x += projectiles[j].xVel * (deltaTicks/1000.f);
		projectiles[j].y += projectiles[j].yVel * (deltaTicks/1000.f);

		if(projectiles[j].x < 0)
		{
			projectiles.erase(it+j);
		}else if(projectiles[j].x+SPRITEWIDTH > MAPWIDTH*TILEWIDTH)
		{
			projectiles.erase(it+j);
		}

		if(projectiles[j].y < 0)
		{
			projectiles.erase(it+j);
		}else if(projectiles[j].y+TILEHEIGHT > MAPHEIGHT*TILEHEIGHT)
		{
			projectiles.erase(it+j);
		}

		int lol;
		for(int i=0;i<MAPSIZE;i++)
		{
			lol = collision(projectiles[j], tileList[i]);
			if(lol)
			{
				switch(tileList[i].type)
				{
					case 5:
						if(lol == 1)
						{projectiles.erase(it+j);}
						else if(lol == 2)
						{projectiles.erase(it+j);}
						else if(lol == 3)
						{projectiles.erase(it+j);}
						else
						{projectiles.erase(it+j);}
				}
			}
		}

		for(unsigned int i=0; i<creatureList.size();i++)
		{
			lol = collision(projectiles[j], creatureList[i]);
			switch(lol)
			{
				case 1:
					creatureList[i].hp -= projectiles[j].damage;
					projectiles.erase(it+j);
					break;
				case 2:
					creatureList[i].hp -= projectiles[j].damage;
					projectiles.erase(it+j);
					break;
				case 3:
					creatureList[i].hp -= projectiles[j].damage;
					projectiles.erase(it+j);
					break;
				case 4:
					creatureList[i].hp -= projectiles[j].damage;
					projectiles.erase(it+j);
					break;
				default:
					break;
			}
		}

		switch(collision(projectiles[j], TheOne))
		{
			case 1:
				TheOne.hp -= projectiles[j].damage;
				projectiles.erase(it+j);
				break;
			case 2:
				TheOne.hp -= projectiles[j].damage;
				projectiles.erase(it+j);
				break;
			case  3:
				TheOne.hp -= projectiles[j].damage;
				projectiles.erase(it+j);
				break;
			case 4:
				TheOne.hp -= projectiles[j].damage;
				projectiles.erase(it+j);
				break;
			default:
				break;
		}

		if(projectiles[j].xVel > 0)
		{
			projectiles[j].direction = 2;
		}else if(projectiles[j].xVel < 0)
		{
			projectiles[j].direction = 4;
		}
		if(projectiles[j].yVel > 0)
		{
			projectiles[j].direction = 1;
		}else if(projectiles[j].yVel < 0)
		{
			projectiles[j].direction = 3;
		}
	}
}


bool map::drawField(SDL_Surface *&screen, player &TheOne)
{
	for(int i=0; i<MAPSIZE; i++)
	{
		if(tileList[i].x>=TheOne.camera.x &&
				tileList[i].x+tileList[i].w<=TheOne.camera.x+TheOne.camera.w &&
				tileList[i].y>=TheOne.camera.y &&
				tileList[i].y+tileList[i].h<=TheOne.camera.y+TheOne.camera.h)
		{
			apply_surface(tileList[i].x - TheOne.camera.x,
					tileList[i].y - TheOne.camera.y,
					tileList[i].texture, screen);
		}
	}
	
	return true;
}


bool map::drawNPC(SDL_Surface *&screen, player &TheOne)
{
	for(unsigned int i=0; i<creatureList.size();i++)
	{
		if(creatureList[i].x>=TheOne.camera.x&&creatureList[i].x+creatureList[i].w<=TheOne.camera.x+TheOne.camera.w&&creatureList[i].y>=TheOne.camera.y&&creatureList[i].y+creatureList[i].h<=TheOne.camera.y+TheOne.camera.h)
		{
			apply_surface(creatureList[i].x - TheOne.camera.x,
					creatureList[i].y - TheOne.camera.y,
					creatureList[i].texture, screen);
		}
	}
	return true;
}


bool map::drawProjectiles(SDL_Surface *&screen, player &TheOne)
{
	for(unsigned int i=0; i<projectiles.size();i++)
	{
		if(projectiles[i].x>=TheOne.camera.x&&projectiles[i].x+projectiles[i].w<=TheOne.camera.x+TheOne.camera.w&&projectiles[i].y>=TheOne.camera.y&&projectiles[i].y+projectiles[i].h<=TheOne.camera.y+TheOne.camera.h)
		{
			if(projectiles[i].texture1 == NULL || projectiles[i].texture2 == NULL
					|| projectiles[i].texture3 == NULL || projectiles[i].texture4
					== NULL)
			{
				cout<< "ERROR SURFACE NULL"<< endl;
			}
			switch(projectiles[i].direction)
			{
				case 1:
					if(!apply_surface(projectiles[i].x - TheOne.camera.x,
						projectiles[i].y - TheOne.camera.y,
						projectiles[i].texture1, screen))
					{
						cout<< "ERROR DRAWING PROJECTILE."<< endl;
						std::string error = SDL_GetError();
						cout<< error<< endl;
					}
					break;
				case 2:
					if(!apply_surface(projectiles[i].x - TheOne.camera.x,
						projectiles[i].y - TheOne.camera.y,
						projectiles[i].texture2, screen))
					{
						cout<< "ERROR DRAWING PROJECTILE."<< endl;
						std::string error = SDL_GetError();
						cout<< error<< endl;
					}
					break;
				case 3:
					if(!apply_surface(projectiles[i].x - TheOne.camera.x,
						projectiles[i].y - TheOne.camera.y,
						projectiles[i].texture3, screen))
					{
						cout<< "ERROR DRAWING PROJECTILE."<< endl;
						std::string error = SDL_GetError();
						cout<< error<< endl;
					}
					break;
				case 4:
					if(!apply_surface(projectiles[i].x - TheOne.camera.x,
						projectiles[i].y - TheOne.camera.y,
						projectiles[i].texture4, screen))
					{
						cout<< "ERROR DRAWING PROJECTILE."<< endl;
						std::string error = SDL_GetError();
						cout<< error<< endl;
					}
					break;
				default:
					break;
			}
		}
	}
	return true;
}


void map::cleanUp()
{
	for(unsigned int i=0;i<creatureList.size();i++)
	{
		if(creatureList[i].hp<=0)
		{
			creatureList.erase(creatureList.begin()+i);
		}
	}
}


// window
//
void window::handle_events(SDL_Event event)
{
	if(windowOK == false)
	{
		return;
	}

	if(event.type == SDL_VIDEORESIZE)
	{
		screen = SDL_SetVideoMode(event.resize.w, event.resize.h, SCREENBPP,
				SDL_SWSURFACE | SDL_RESIZABLE);

		if(screen == NULL)
		{
			windowOK = false;
			return;
		}
	}else if((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_F12))
	{
		toggle_fullscreen();
	}else if(event.type == SDL_VIDEOEXPOSE)
	{
		if(SDL_Flip(screen) == -1)
		{
			windowOK = false;
			return;
		}
	}else if(event.type == SDL_ACTIVEEVENT)
	{
		if(event.active.state & SDL_APPACTIVE)
		{
			if(event.active.gain == 0)
			{
				SDL_WaitEvent(&event);
			}
		}
	}
}


void window::toggle_fullscreen()
{
	if(windowed == true)
	{
		screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, SCREENBPP,
				SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN);

		if(screen == NULL)
		{
			windowOK = false;
			return;
		}

		windowed = false;
	}else if(windowed == false)
	{
		screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, SCREENBPP,
				SDL_SWSURFACE | SDL_RESIZABLE);

		if(screen == NULL)
		{
			windowOK = false;
			return;
		}

		windowed = true;
	}
}


bool window:: error()
{
	return !windowOK;
}


// timer
//
void timer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}


void timer::stop()
{
	started = false;
	paused = false;
}


void timer::pause()
{
	if((started == true) && (paused == false))
	{
		paused = true;
		pausedTicks = SDL_GetTicks() - startTicks;;
	}
}


void timer::unpause()
{
	if(paused == true)
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}


int timer::get_ticks()
{
	if(started == true)
	{
		if(paused == true)
		{
			return pausedTicks;
		}else
		{
			return SDL_GetTicks() - startTicks;
		}
	}
	return 0;
}


bool timer::is_started()
{
	return started;
}


bool timer::is_paused()
{
	return paused;
}


// container
//
bool container::list(window &mainWindow)
{
	int j = 0;
	SDL_Event event;
	timer fps;
	tile select;
	select.x = 0;
	select.y = 0;
	select.w = 32;
	select.h = 32;
	select.texture = load_image(filePath("data/outline.bmp"));
	bool quit(false);
	Uint32 windowColour = SDL_MapRGB(mainWindow.screen->format, 255,255,255);

	while(!quit)
	{
		fps.start();
		while(SDL_PollEvent(&event))
		{
			mainWindow.handle_events(event);
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		Uint8 *keystates = SDL_GetKeyState(NULL);
		if(keystates[SDLK_q])
		{quit = true;}
		else if(keystates[SDLK_UP]) // FIX CODE HERE; ADD J MODIFIER?
		{
			select.y -= SPRITEHEIGHT*2;
			if(select.y<0){select.y+=SPRITEHEIGHT*2;}
		}else if(keystates[SDLK_DOWN]) // HERE TOO
		{
			select.y += SPRITEHEIGHT*2;
			if(select.y+select.h>SCREENHEIGHT){select.y-=SPRITEHEIGHT*2;}
		}else if(keystates[SDLK_RIGHT])
		{
			select.x += SPRITEWIDTH*2;
			if(select.x+select.w>SCREENWIDTH)
			{
				select.x -= SPRITEWIDTH*2;
				j += 1;
			}
		}else if(keystates[SDLK_LEFT])
		{
			select.x -= SPRITEWIDTH*2;
			if(select.x-select.w<0)
			{
				select.x += SPRITEWIDTH*2;
				j += 1;
			}
		}else if(keystates[SDLK_SPACE])
		{
		}

		SDL_FillRect(mainWindow.screen, NULL, windowColour);
		int itemX(0), itemY(0);
		for(unsigned int i=0;i<contents.size();i++)
		{
			apply_surface(itemX, itemY, contents[i]->texture,
					mainWindow.screen);
			if(itemX+contents[i]->w+SPRITEWIDTH > SCREENWIDTH)
			{
				itemY += contents[i]->w+SPRITEWIDTH > SCREENWIDTH;
				itemX = 0;
			}else
			{
				itemX += contents[i]->w+SPRITEWIDTH;
			}
		}
		apply_surface(select.x, select.y, select.texture, mainWindow.screen);
		SDL_Flip(mainWindow.screen);

		if(fps.get_ticks() < 1000/12)
		{
			SDL_Delay((1000/12) - fps.get_ticks());
		}
	}

	return true;
}


void container::put(item newItem)
{
	item *element = &newItem;
	contents.push_back(element);
	return;
}


item* container::get(int index)
{
	return contents[index];
}


void container::clear()
{
	contents.erase(contents.begin(), contents.end());
	return;
}



// UNASSORTED FUNCTIONS

SDL_Surface *load_image(std::string filename)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	loadedImage = IMG_Load(filename.c_str());

	if(loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		Uint32 colorkey = SDL_MapRGB(
				SDL_DisplayFormat(optimizedImage)->format,0xFF, 0x0, 0xFF);
		SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);		
		SDL_FreeSurface(loadedImage);
	}

	return optimizedImage;
}


bool apply_surface(int x, int y, SDL_Surface *&source, SDL_Surface *&dest)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	if(SDL_BlitSurface(source, NULL, dest, &offset) > -1)
	{
		return true;
	}
	return false;
}


bool loadSave(std::string saveAddress, player &TheOne, map &map1)
{
	ifstream save;
	save.open(saveAddress.c_str());
	if(save.is_open())
	{
		int offset;
		save>> map1.name;
		save.ignore();
		if(map1.readMap() && map1.loadStage())
		{
			save>> offset;
			TheOne.x = offset;
			save>> offset;
			TheOne.y = offset;
			save.close();
			return true;
		}else
		{
			save.close();
			return false;
		}
	}else
	{
		return false;
	}
}


bool createSave(std::string saveAddress, std::string mapName, player &TheOne)
{
	ofstream save;
	save.open(saveAddress.c_str());
	if(save.is_open())
	{
		save<< mapName<< endl;
		save<< TheOne.x<< " ";
		save<< TheOne.y;
		save.close();

		return true;
	}
	return false;
}


int collision(thing thingOne, thing thingTwo)
{
	if(thingOne.xVel>0)
	{
		if(thingOne.x+thingOne.w>thingTwo.x&&thingOne.x+thingOne.w<thingTwo.x+thingTwo.w&&((thingOne.y>thingTwo.y&&thingOne.y<thingTwo.y+thingTwo.h)||(thingOne.y+thingOne.h<thingTwo.y+thingTwo.h&&thingOne.y+thingOne.h>thingTwo.y)||(thingOne.y==thingTwo.y||thingOne.y+thingOne.h==thingTwo.y+thingTwo.h)))
		{
			return 1;
		}
	}else if (thingOne.xVel<0)
	{
		if(thingOne.x<thingTwo.x+thingTwo.w&&thingOne.x>thingTwo.x&&((thingOne.y>thingTwo.y&&thingOne.y<thingTwo.y+thingTwo.h)||(thingOne.y+thingOne.h<thingTwo.y+thingTwo.h&&thingOne.y+thingOne.h>thingTwo.y)||(thingOne.y==thingTwo.y||thingOne.y+thingOne.h==thingTwo.y+thingTwo.h)))
		{
			return 2;
		}
	}

	if(thingOne.yVel>0)
	{
		if(thingOne.y+thingOne.h>thingTwo.y&&thingOne.y+thingOne.h<thingTwo.y+thingTwo.h&&((thingOne.x>thingTwo.x&&thingOne.x<thingTwo.x+thingTwo.w)||(thingOne.x+thingOne.w<thingTwo.x+thingTwo.w&&thingOne.x+thingOne.w>thingTwo.x)||(thingOne.x==thingTwo.x||thingOne.x+thingOne.w==thingTwo.x+thingTwo.w)))
		{
			return 3;
		}
	}else if(thingOne.yVel<0)
	{
		if(thingOne.y<thingTwo.y+thingTwo.h&&thingOne.y>thingTwo.y&&((thingOne.x>thingTwo.x&&thingOne.x<thingTwo.x+thingTwo.w)||(thingOne.x+thingOne.w<thingTwo.x+thingTwo.w&&thingOne.x+thingOne.w>thingTwo.x)||(thingOne.x==thingTwo.x||thingOne.x+thingOne.w==thingTwo.x+thingTwo.w)))
		{
			return 4;
		}
	}

	return 0;
}


bool getFlag(unsigned int flag, int which)
{
	unsigned int bitmap = 1;
	flag = flag >>which;
	flag = flag & bitmap;
	if(flag == 1)
	{return true;}
	return false;
}


void setFlag(unsigned int &flag, int which, bool state)
{
	if(state)
	{
		unsigned int bitmap = 1;
		bitmap = bitmap << which;
		flag = flag | bitmap;
	}else
	{
		unsigned int bitmap = 0xFFFFFFFE;
		for(int i=0;i<which;i++)
		{
			bitmap = bitmap<<1;
			bitmap++;
		}
		flag = flag & bitmap;
	}
}
