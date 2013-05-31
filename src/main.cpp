#include "os.hxx"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "init.hxx"
#include "base-classes.hxx"
#include "monsters.hxx"
#include "tools.hxx"


int main(int argc, char* args[])
{
	bool quit = false;
	window mainWindow;
	map map1;
	SDL_Event event;
	// TTF_Font *coreFont;
	player player1;
	timer walkRate;
	timer fps;

	// TESTING BEGIN
	weapon testWeapon;
	testWeapon.damage = 5;
	testWeapon.range = 17;
	testWeapon.texture = load_image("../data/sprites/testWeapon1.bmp");
	testWeapon.texture1 = load_image("../data/sprites/testWeapon1.bmp");
	testWeapon.texture2 = load_image("../data/sprites/testWeapon2.bmp");
	testWeapon.texture3 = load_image("../data/sprites/testWeapon3.bmp");
	testWeapon.texture4 = load_image("../data/sprites/testWeapon4.bmp");
	/*
	for(int i=0;i<5;i++)
	{
		testWeapon.texture[i] = 
			load_image("../data/sprites/testWeapon"+i+".bmp");
	}
	*/
	// player1.primaryWeapon = testWeapon;
	weapon testRanged;
	testRanged.damage = 2;
	testRanged.texture = load_image("../data/sprites/testWeapon2.bmp");
	testRanged.texture1 = load_image("../data/sprites/testWeapon2.bmp");
	testRanged.texture2 = load_image("../data/sprites/testWeapon2.bmp");
	testRanged.texture3 = load_image("../data/sprites/testWeapon2.bmp");
	testRanged.texture4 = load_image("../data/sprites/testWeapon2.bmp");
	/*
	for(int i=0;i<5;i++)
	{
		testRanged.texture[i] = load_image("../data/sprites/testWeapon2.bmp");
	}
	*/
	testRanged.ranged = true;
	player1.primaryWeapon = testRanged;
	for(int i=0;i<20;i++)
	{
		ammo testAmmo;		
		testAmmo.damage = 5;
		testAmmo.texture = load_image("../data/sprites/testWeapon1.bmp");
		testAmmo.texture1 = load_image("../data/sprites/testWeapon1.bmp");
		testAmmo.texture2 = load_image("../data/sprites/testWeapon2.bmp");
		testAmmo.texture3 = load_image("../data/sprites/testWeapon3.bmp");
		testAmmo.texture4 = load_image("../data/sprites/testWeapon4.bmp");
		/*
		for(int i=0;i<5;i++)
		{
			testAmmo.texture[i] = load_image("../data/sprites/testWeapon"
					+i+".bmp");
		}
		*/
		player1.quiver.push_back(testAmmo);
	}
	// item testItem;
	// testItem.texture = load_image("data\\testWeapon1.bmp");
	weapon *testPointer1 = &testWeapon;
	weapon *testPointer2 = &testRanged;
	player1.inventory.push_back(testPointer1);
	player1.inventory.push_back(testPointer2);
	// TESTING END


	if(init(mainWindow, player1, map1) == false)
	{
		return 1;
	}

	walkRate.start();
	while(quit == false)
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
		
		player1.weapon_drawn = false;
		Uint8 *keystates = SDL_GetKeyState(NULL);
		player1.handleInput(keystates);
		if(player1.inventoryFlag)
		{
			walkRate.pause();
			fps.pause();
			player1.checkInventory(mainWindow);
			walkRate.unpause();
			fps.unpause();
		}
		player1.move(map1, walkRate.get_ticks());
		for(unsigned int i=0;i<map1.creatureList.size();i++)
		{
			map1.creatureList[i].think(player1);
			map1.creatureList[i].move(map1, player1, walkRate.get_ticks());
		}
		map1.projectileLoop(player1, walkRate.get_ticks());
		walkRate.start();
		player1.set_camera();
		map1.drawField(mainWindow.screen, player1);
		map1.drawNPC(mainWindow.screen, player1);
		player1.show(mainWindow.screen);
		if(player1.weapon_drawn == true){
			player1.draw_weapon(map1, mainWindow);
		}
		map1.drawProjectiles(mainWindow.screen, player1);
		map1.cleanUp();

		if(SDL_Flip(mainWindow.screen) == -1)
		{
			return 1;
		}

		if(fps.get_ticks() < 1000/FRAMESPERSECOND)
		{
			SDL_Delay((1000/FRAMESPERSECOND) - fps.get_ticks());
		}
	}

	clean_up(mainWindow, map1, player1);
	return 0;
}
