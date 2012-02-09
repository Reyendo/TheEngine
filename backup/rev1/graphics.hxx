#ifndef GRAPHICS
#define GRAPHICS

#define STRICT
#include <windows.h>
#include "resource.hxx"
#include "misc.hxx"


HBITMAP InitializeBlocks(HWND hwnd, HBITMAP block, int image)
{
	block = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(image));
	if(block == NULL)
	{
		MessageBox(hwnd, "Could not load block!", "Error", 
				MB_OK | MB_ICONEXCLAMATION);
	}
	return block;
}


void DrawBlock(HDC hdc, HBITMAP block, int x, int y)
{
	BITMAP bm;

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, block);

	GetObject(block, sizeof(bm), &bm);

	BitBlt(hdc, bm.bmWidth*x, bm.bmHeight*y, bm.bmWidth, bm.bmHeight, 
			hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
}


int DrawField(HWND hwnd, HDC hdc, block lol[], int layout[])
{
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	int iterator = 0;

	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rcClient.right, 
			rcClient.bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);
	FillRect(hdcBuffer, &rcClient,(HBRUSH) GetStockObject(WHITE_BRUSH));

	for(int k=0; k<25;k++)
	{
		for(int j=0; j<25; j++)
		{
			DrawBlock(hdcBuffer, lol[layout[iterator]].texture, j, k);
			iterator++;
		}
	}

	BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcBuffer, 0, 0, SRCCOPY);
	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}



int Movement(HWND hwnd, player &TheBoy, block lol[], int layout[], int x, int y)
{
	int direction;
	if((direction=x-TheBoy.x)!=0)
	{
		if(direction=1)
		{TheBoy.direction=2;}
		else
		{TheBoy.direction=4;}
	}
	else if((direction=y-TheBoy.y)!=0)
	{
		if(direction=1)
		{TheBoy.direction=3;}
		else
		{TheBoy.direction=1;}
	}

	switch(lol[layout[y*25+x]].state)
	{
		case 0:
			TheBoy.x=x;
			TheBoy.y=y;
			break;
		case 1:
			break;
		case 2:
			TheBoy.texture = InitializeBlocks(hwnd, TheBoy.texture, 
					IDB_PLAYER2);
			TheBoy.x=x;
			TheBoy.y=y;
		default:
			return 3;
	}
	return 1;
}


#endif
