#ifndef BASE_CLASSES_H
#define BASE_CLASSES_H


#include "os.hxx"
#include <iostream>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


using namespace std;


#define MAPWIDTH 100
#define MAPHEIGHT 100
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
#define PLAYERREACH 16

//
// Possibly will change later.
//
// DEFINE ITEM TYPES
//

#define UNASSIGNED	0
#define WEAPON		1


// Add namespace to isolate enum
enum{solid, container};

class thing;
struct tile;
class container;
class item;
class weapon;
class ammo;
class creature;
class player;
class npc;
class enemy;
class map;
class window;
class timer;




class thing
{
	private:
	public:
		int x, y, w, h;
		int xVel, yVel;

		thing();
};


struct tile: public thing
{
	private:
	public:
		int type;
		std::string data;
		SDL_Surface *texture;

		tile();
};


class container: public thing
{
	public:
		SDL_Surface * texture;
		std::vector<item*> contents;

		container();
		bool list(window &mainWindow);
		void put(item newItem);
		item* get(int index);
		void clear();
};


class item: public thing
{
	private:
	public:
		SDL_Surface* texture;
		SDL_Surface* texture1;
		SDL_Surface* texture2;
		SDL_Surface* texture3;
		SDL_Surface* texture4;
		// Change type? V
		int type;

		item();
};


class weapon: public item
{
	private:
	public:
		int damage;
		int effect;
		int range;
		bool ranged;

		weapon();
};


class ammo: public item
{
	private:
	public:
		int direction;
		int damage;
		int effect;

		ammo();
};


class creature: public thing
{
	private:
	public:
		std::string name;
		int race;
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
		SDL_Surface* texture;
		SDL_Surface* texture1;
		SDL_Surface* texture2;
		SDL_Surface* texture3;
		SDL_Surface* texture4;
		SDL_Rect camera;
		int direction;
		weapon primaryWeapon;

		creature();
		void show(SDL_Surface *&screen);
};


class player: public creature
{
	private:
	public:
		bool weapon_drawn;
		bool inventoryFlag;
		bool interactFlag;
		int keysense;
		
		std::vector<item*> inventory;
		std::vector<ammo> quiver;

		player();
		bool handleInput(Uint8 *keystates);
		bool move(map &world, Uint32 deltaTicks);
		void draw_weapon(map &map1, window &mainWindow);
		void checkInventory(window &mainWindow);
		void interact(map &map1, window &mainWindow);
		void set_camera();
		void show(SDL_Surface *&screen);
};


class npc: public creature
{
	private:
	public:
		std::string name;

		npc();
		virtual bool think(player &TheOne);		
		virtual void move(map &map1, player &TheOne, Uint32 deltaTicks);
};


class enemy: public npc
{
	private:
	public:
		enemy();
};


class map
{
	private:
	public:
		std::string name;
		std::string texName[50];
		unsigned int tileType[50];		
		std::string tileData[50];
		SDL_Surface* texture[50];
		int layout[MAPSIZE];
		tile tileList[MAPSIZE];
		std::vector<npc> creatureList;
		std::vector<ammo> projectiles;

		map();
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
TTF_Font *load_font(const char* file, int ptsize);
bool apply_surface(int x, int y, SDL_Surface *&source, SDL_Surface *&dest);
bool loadSave(std::string saveAddress, player &TheOne, map &world);
bool createSave(std::string saveAddress, std::string mapName, player &TheOne);
// ADD COLLISION FUNCTION WITH RANGE PARAMETER
int collision(thing thingOne, thing thingTwo);
bool getFlag(unsigned int flag, int which);
void setFlag(unsigned int &flag, int which, bool state);



#endif
