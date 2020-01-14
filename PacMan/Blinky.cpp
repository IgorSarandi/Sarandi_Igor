#include "Blinky.h"



Blinky::Blinky()
{
}


Blinky::~Blinky()
{
}

std::pair<char, int> Blinky::Figure()
{
	if (this->GetStatus() != 3)
	{
		return std::make_pair(char(2), 76);
	}
	else
	{
		return std::make_pair(char(2), 73);
	}
}