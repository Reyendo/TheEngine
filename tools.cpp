#include "tools.hxx"



#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <windows.h>





void returnError()
{
	int error = GetLastError();
	char string[8];
	itoa(error, string, 10);
	const char* data = string;
	MessageBox(NULL, data, "ALERT", MB_OK);
}


void msgBoxInt(int x)
{
	char data[5];
	itoa(x, data, 10);
	const char* msg = data;
	MessageBox(NULL, msg, "Alert!", MB_OK);
}






/*
bool map::drawNPC(SDL_Surface *&screen, player &TheOne)
{
	for(int i=0; i<creatureList.size();i++)
	{
		if(creatureList[i].x>=TheOne.camera.x&&creatureList[i].x+creatureList[i].w<=TheOne.camera.x+TheOne.camera.w&&creatureList[i].y>=TheOne.camera.y&&creatureList[i].y+creatureList[i].h<=TheOne.camera.y+TheOne.camera.h)
		{
			apply_surface(creatureList[i].x - TheOne.camera.x,
					creatureList[i].y - TheOne.camera.y,
					creatureList[i].texture, screen);
		}
	}
}
*/
