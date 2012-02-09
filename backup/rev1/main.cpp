#define STRICT
#include <windows.h>
#include <iostream>
#include "resource.hxx"
#include "graphics.hxx"
#include "misc.hxx"

using namespace std;

// CONSTANTS ////////////////////////////////////

const char g_szClassName[] = "classname";
const DWORD WM_LOADSTAGE = WM_USER+1;
HDC g_hdcMasterBuffer;
HBITMAP g_hbmMasterBuffer;
RECT g_rcClient;
const int ID_TIMER = 1;
block rofl[20];
int grid[MAPSIZE];
player Player1;


// Window Proc /////////////////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			{
				UINT ret;				
				Player1.texture = InitializeBlocks(hwnd, Player1.texture, 
						IDB_PLAYER);
				Player1.x = 0;
				Player1.y = 0;
				ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
				if(ret == 0)
					MessageBox(hwnd,"Could not set timer!", "Error", 
							MB_OK|MB_ICONEXCLAMATION);
				PostMessage(hwnd, WM_LOADSTAGE, 0, 0);
			}
			break;

		case WM_LOADSTAGE:
			{
				if(readMap(hwnd,"data\\test.mapx",rofl,grid)==0)
				{
					MessageBox(hwnd, "readMap failed!","Error", MB_OK);
				}

				for(int i=0;i<50;i++)
				{
					if(rofl[i].texID!=0)
					{
						rofl[i].texture=InitializeBlocks(hwnd,rofl[i].texture,
								rofl[i].texID);
					}else{break;}
				}
				InvalidateRect(hwnd, NULL, TRUE);				
			}
			break;

		case WM_PAINT:
			{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			DrawField(hwnd, hdc, rofl, grid);
			DrawBlock(hdc, Player1.texture, Player1.x, Player1.y);
			EndPaint(hwnd, &ps);
			}
			break;

		case WM_KEYDOWN:
			{
			int nextX = Player1.x;
			int nextY = Player1.y;

			switch(wParam)
			{
				case VK_RIGHT:
					if(Player1.x<25){
						nextX++;
						Movement(hwnd, Player1, rofl, grid, nextX, nextY);
						InvalidateRect(hwnd, NULL, TRUE);
					}
					break;
				case VK_LEFT:
					if(Player1.x>0){
						nextX--;
						Movement(hwnd, Player1, rofl, grid, nextX, nextY);
						InvalidateRect(hwnd, NULL, TRUE);
					}
					break;
				case VK_UP:
					if(Player1.y>0){
						nextY--;
						Movement(hwnd, Player1, rofl, grid, nextX, nextY);
						InvalidateRect(hwnd, NULL, TRUE);
					}
					break;
				case VK_DOWN:
					if(Player1.y<25){
						nextY++;
						Movement(hwnd, Player1, rofl, grid, nextX, nextY);
						InvalidateRect(hwnd, NULL, TRUE);
					}
					break;
			}
			}
			break;

		case WM_TIMER:
			{
			}
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
		LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	// Registering the Window Class
	wc.cbSize 		= sizeof(WNDCLASSEX);
	wc.style		= 0;
	wc.lpfnWndProc	= WndProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInstance;
	wc.hIcon		= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground= (HBRUSH) (COLOR_WINDOW+1);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= g_szClassName;
	wc.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", 
				MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Creating Window
	hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			g_szClassName,
			"The title of my window",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
			NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", 
				MB_ICONEXCLAMATION | MB_OK);

	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
return Msg.wParam;
}
