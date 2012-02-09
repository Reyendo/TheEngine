#ifndef MISCELLANEOUS
#define MISCELLANEOUS

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "resource.hxx"

using namespace std;

#define SPRITESIZE 32
#define MAPSIZE 625


class player
{
	private:
		// race?
		int hp;
		int mp;
		int status;

		int strength;
		int endurance;
		int agility;
		int intelligence;
		int charisma;
		int wisdom;
		int will;

	public:
		char* name;
		HBITMAP texture;
		int direction;
		int x;
		int y;
};

class creature
{
	private:
		int hp;
		int mp;
		int status;

		int strength;
		int endurance;
		int agility;
		int intelligence;
		int charisma;
		int wisdom;
		int will;
	public:
		char* name;

};

struct block
{
	int texID;
	int state;
	HBITMAP texture;
};

class room
{
	private:
		int width;
		int height;
		int walls;
};

class weapon
{
	private:
		bool type; // physical/magical
		int damage;
		int range;
		int status;
};


int readMap(HWND hwnd, const char* data, block lol[], int layout[])
{
	ifstream map;
	map.open(data);
	if(map.is_open())
	{
		int i=0;
		string line, token;
		// stringstream iss;
		while(getline(map, line))
		{
			if(i<50){
				for(int j=0;j<line.length(); j++)
				{
					if(line[j]==',')
					{
						//MessageBox(hwnd, token.c_str(), "ResourceID",MB_OK);	
						lol[i].texID=atoi(token.c_str());
						token.erase(0, token.length());
						i++;
					}else{token+=line[j];}
				}
			}else if(i<100)
			{
				for(int j=0;j<line.length();j++)
				{
					if(line[j]==',')
					{
						// MessageBox(hwnd, token.c_str(), "ALERT", MB_OK);
						lol[i-50].state=atoi(token.c_str());
						token.erase(0, token.length());
						i++;
					}else{token+=line[j];}
				}
			}else 
			{
				for(int j=0;j<line.length();j++)
				{
					if(line[j]==',')
					{
						layout[i-100]=atoi(token.c_str());
						token.erase(0, token.length());
						i++;
					}else{token+=line[j];}
				}
			}
		}
		map.close();
	}else return 0;
	return 1;
}

#endif
