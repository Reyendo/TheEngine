#ifndef MISC_H
#define MISC_H


#include <iostream>
#include <string>
#include <vector>
#include <SDL\SDL.h>


using namespace std;


#define MAPWIDTH 100
#define MAPLENGTH 100
#define MAPSIZE 10000
#define SPRITEHEIGHT 32
#define SPRITEWIDTH 32
#define PLAYERVELOCITY 500
#define FRAMESPERSECOND 80
#define TILEWIDTH 32
#define TILEHEIGHT 32
#define SCREENWIDTH 640
#define SCREENHEIGHT 416
#define SCREENBPP 32



class thing;
struct tile;
class item;
class weapon;
class creature;
class player;
class npc;
class map;
class window;
class timer;




class thing
{
	public:
		int x, y, w, h;
		int xVel, yVel;

		thing();
};


struct tile: public thing
{
	public:
		int type;
		std::string info;
		SDL_Surface *texture;
};


class item: public thing
{
	public:
		SDL_Surface *texture;
};


class ammo: public item
{
	public:
		int direction;
		int damage;
		int effect;
		SDL_Surface* texture1;
		SDL_Surface* texture2;
		SDL_Surface* texture3;
		SDL_Surface* texture4;
};


class weapon: public item
{
	public:
		int damage;
		int effect;
		int range;
		bool ranged;
		SDL_Surface* texture1;
		SDL_Surface* texture2;
		SDL_Surface* texture3;
		SDL_Surface* texture4;

		weapon();
};


class map
{
	private:
	public:
		std::string name;
		std::string texName[50];
		int tileType[50];		
		std::string tileInfo[50];
		SDL_Surface* texture[50];
		int layout[MAPSIZE];
		tile tileList[MAPSIZE];
		std::vector<npc> creatureList;
		std::vector<ammo> projectiles;

		bool readMap();
		bool loadStage();
		void projectileLoop(player &TheOne, Uint32 deltaTicks);
		bool drawField(SDL_Surface *&screen, player &TheOne);
		bool drawNPC(SDL_Surface *&screen, player &TheOne);
		bool drawProjectiles(SDL_Surface *&screen, player &TheOne);
		void cleanUp();
};


class window
{
	private:
		bool windowed;
		bool windowOK;

	public:
		SDL_Surface *screen;
		window();
		void handle_events(SDL_Event event);
		void toggle_fullscreen();
		bool error();
};


class creature: public thing
{
	protected:

	public:
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

		char* name;
		SDL_Surface* texture;
		SDL_Rect camera;
		int direction;
		weapon primaryWeapon;
		void show(SDL_Surface *&screen);		

		creature();
};


class npc: public creature
{
	private:
	public:
		std::string name;

		bool think(player &TheOne);		
		void move(map &map1, player &TheOne, Uint32 deltaTicks);

		npc();	
};


class enemy: public npc
{
	private:
	public:
		enemy();
};


class player: public creature
{
	public:
		bool weapon_drawn;
		bool inventoryFlag;
		int keysense;
		
		std::vector<item*> inventory;
		std::vector<ammo> quiver;

		player();
		bool handleInput(Uint8 *keystates);
		bool move(map &world, Uint32 deltaTicks);
		void draw_weapon(map &map1, window &mainWindow);
		void checkInventory(window &mainWindow);		
		void set_camera();
		void show(SDL_Surface *&screen);
};


class timer
{
	private:
		int startTicks;
		int pausedTicks;
		bool paused;
		bool started;

	public:
		timer();
		void start();
		void stop();
		void pause();
		void unpause();
		int get_ticks();
		bool is_started();
		bool is_paused();
};




SDL_Surface *load_image(std::string filename);
void apply_surface(int x, int y, SDL_Surface *&source, SDL_Surface *&dest);
bool loadSave(std::string saveAddress, player &TheOne, map &world);
bool createSave(std::string saveAddress, std::string mapName, player &TheOne);
int collision(thing thingOne, thing thingTwo);



#endif
