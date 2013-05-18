#ifndef MONSTERS_H
#define MONSTERS_H

#include "base-classes.hxx"


class spider: public enemy
{
	spider();
	bool think(player &TheOne);
};


#endif
