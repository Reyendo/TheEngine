#include "tools.hxx"



#include "os.hxx"
#ifdef WINDOWS
#include <SDL\SDL.h>
#include <windows.h>
#endif
#ifdef LINUX 
#include <SDL/SDL.h>
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>


#ifdef WINDOWS

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

std::string filePath(const char* fn)
{
	int i = 0;
	std::string newFn;
	while(1)
	{
		if(fn[i]=='/')
		{
			newFn+="\\\\";
		}else if(fn[i]=='\0')
		{
			return newFn;
		}else
		{
			newFn+=fn[i];
		}

		i++;
	}
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

std::string filePath(const char* fn)
{
	int i = 0;
	std::string newFn;
	while(1)
	{
		if(fn[i]=='\\')
		{
			newFn+='/';
			i++;
		}else if(fn[i]=='\0')
		{
			return newFn;
		}else
		{
			newFn+=fn[i];
		}

		i++;
	}

	return NULL;
}

#endif
