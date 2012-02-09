#include "monsters.hxx"


spider::spider()
{}


bool spider::think(player &TheOne)
{
	int iterator = 0;
	bool lateral = false;
	if(iterator < 3)
	{
		if(lateral)
		{
			xVel = PLAYERVELOCITY;
			yVel = 0;
		}else
		{
			yVel = PLAYERVELOCITY;
			xVel = 0;
		}
		iterator ++;
	}else
	{
		iterator = 0;
		lateral = !lateral;
	}

	return true;
}
