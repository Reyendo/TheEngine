#include "tools.hxx"



#include "os.hxx"
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>


#ifdef WINDOWS

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

#endif

#ifdef LINUX

void returnError()
{
	return;
}

void msgBoxInt(int x)
{
	return;
}

#endif
