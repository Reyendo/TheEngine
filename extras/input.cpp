#include <SDL/SDL.h>


bool input()
{
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_Quit)
		{
			return false;
		}
	}
}
