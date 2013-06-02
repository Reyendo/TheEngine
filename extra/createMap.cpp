#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

#define tileNameSize 32
#define tileDataSize 32

int main()
{
	ofstream map;
	map.open("../data/maps/testMap.mapx",ios::binary);
	if(map.is_open())
	{
		char tileNameBuffer[tileNameSize] = {'\0'};
		char tileDataBuffer[tileDataSize] = {'\0'};
		int tileType = 0;

		memcpy(tileNameBuffer,"testblock0.bmp",sizeof("testblock0.bmp"));
		tileType = 0;
		memset(tileDataBuffer,0,tileDataSize);		
		map.write(tileNameBuffer,tileNameSize);
		map.write((char *)&tileType,sizeof(int));
		map.write(tileDataBuffer,tileDataSize);

		memset(tileNameBuffer,0,tileNameSize);
		tileType = 0;
		memset(tileDataBuffer,0,tileDataSize);

		memcpy(tileNameBuffer,"testblock1.bmp",sizeof("testblock1.bmp"));
		tileType = 1;
		memset(tileDataBuffer,0,tileDataSize);		
		map.write(tileNameBuffer,tileNameSize);
		map.write((char *)&tileType,sizeof(int));
		map.write(tileDataBuffer,tileDataSize);

		memset(tileNameBuffer,0,tileNameSize);
		tileType = 0;
		memset(tileDataBuffer,0,tileDataSize);

		memcpy(tileNameBuffer,"testblock2.bmp",sizeof("testblock2.bmp"));
		tileType = 2;
		memset(tileDataBuffer,0,tileDataSize);		
		map.write(tileNameBuffer,tileNameSize);
		map.write((char *)&tileType,sizeof(int));
		map.write(tileDataBuffer,tileDataSize);

		memset(tileNameBuffer,0,tileNameSize);
		tileType = 0;
		memset(tileDataBuffer,0,tileDataSize);

		memcpy(tileNameBuffer,"testblock3.bmp",sizeof("testblock3.bmp"));
		tileType = 3;
		memset(tileDataBuffer,0,tileDataSize);		
		map.write(tileNameBuffer,tileNameSize);
		map.write((char *)&tileType,sizeof(int));
		map.write(tileDataBuffer,tileDataSize);

		memset(tileNameBuffer,0,tileNameSize);
		tileType = 0;
		memset(tileDataBuffer,0,tileDataSize);

		memcpy(tileNameBuffer,"testblock4.bmp",sizeof("testblock4.bmp"));
		tileType = 1;
		memset(tileDataBuffer,0,tileDataSize);		
		map.write(tileNameBuffer,tileNameSize);
		map.write((char *)&tileType,sizeof(int));
		map.write(tileDataBuffer,tileDataSize);

		memset(tileNameBuffer,0,tileNameSize);
		tileType = 0;
		memset(tileDataBuffer,0,tileDataSize);

		memcpy(tileNameBuffer,"testblock5.bmp",sizeof("testblock5.bmp"));
		tileType = 6;
		memcpy(tileDataBuffer,"defMap2.mapx",sizeof("defMap2.mapx"));
		map.write(tileNameBuffer,tileNameSize);
		map.write((char *)&tileType,sizeof(int));
		map.write(tileDataBuffer,tileDataSize);

		memset(tileNameBuffer,0,tileNameSize);
		tileType = 1;
		memset(tileDataBuffer,0,tileDataSize);

		for(int i=0;i<44;i++)
		{
			map.write(tileNameBuffer,tileNameSize);
			map.write((char *)&tileType,sizeof(int));
			map.write(tileDataBuffer,tileDataSize);
		}

		int grid[100][100] = {0};
		grid[0][99] = 5;
		grid[1][15] = 4;
		grid[1][20] = 4;
		grid[1][99] = 5;
		grid[2][0] = 4;
		grid[2][99] = 5;
		grid[3][15] = 4;
		grid[3][99] = 4;
		grid[99][99] = 4;

		map.write((char *)grid,sizeof(int)*(100)*(100));


		/*
		char tileNameBuffer[tileNameSize] = {NULL};
		memcpy(tileNameBuffer,"testblock0.bmp",sizeof("testblock0.bmp"));
		map.write(tileNameBuffer,tileNameSize);
		memset(tileNameBuffer,0,tileNameSize);
		memcpy(tileNameBuffer,"testblock1.bmp",sizeof("testblock1.bmp"));
		map.write(tileNameBuffer,tileNameSize);
		memset(tileNameBuffer,0,tileNameSize);
		memcpy(tileNameBuffer,"testblock2.bmp",sizeof("testblock2.bmp"));
		map.write(tileNameBuffer,tileNameSize);
		memset(tileNameBuffer,0,tileNameSize);
		memcpy(tileNameBuffer,"testblock3.bmp",sizeof("testblock3.bmp"));
		map.write(tileNameBuffer,tileNameSize);
		memset(tileNameBuffer,0,tileNameSize);
		memcpy(tileNameBuffer,"testblock4.bmp",sizeof("testblock4.bmp"));
		map.write(tileNameBuffer,tileNameSize);
		memset(tileNameBuffer,0,tileNameSize);
		memcpy(tileNameBuffer,"testblock5.bmp",sizeof("testblock5.bmp"));
		map.write(tileNameBuffer,tileNameSize);
		memset(tileNameBuffer,0,tileNameSize);

		for(int i=0;i<44;i++)
		{
			map.write(tileNameBuffer,tileNameSize);
		}

		int tileType = 0;
		map.write(tileType,sizeof(int));
		tileType = 1;
		map.write(tileType,sizeof(int));
		tileType = 2;
		map.write(tileType,sizeof(int));
		tileType = 3;
		map.write(tileType,sizeof(int));
		tileType = 1;
		map.write(tileType,sizeof(int));
		tileType = 6;
		map.write(tileType,sizeof(int));
		tileType = 1;

		for(int i=0;i<44;i++) { map.write(tileType,sizeof(int)); } char tileDataBuffer[tileDataSize] = {NULL}; memset(tileDataBuffer,0,tileDataSize); for(int i=0;i<5;i++)
		{
			map.write(tileDataBuffer,tileDataSize);
		}
		memcpy(tileDataBuffer,"defMap2.mapx",sizeof("defMap2.mapx"));
		memset(tileDataBuffer,0,tileDataSize);
		for(int i=0;i<44;i++)
		{
			map.write(tileDataBuffer,tileDataSize);
		}
		memset(tileDataBuffer,0,tileDataSize);
		*/
		
		cout<< "File created successfully"<< endl;
		map.close();
	}else
	{
		cout<< "Error: File did not open correctly"<< endl;
	}
	cin.get();

	return 0;
}
