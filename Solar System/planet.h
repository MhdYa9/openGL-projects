#pragma once
#include "color.h"
#define db double

class planet
{
public:
	planet(db radius, color& c,bool redflag = false, bool greenflag = false, bool blueflag = false);
	void planettext(db radius, db stacks, db sectores);
};

