#include "Border.h"



Border::Border()
{
}


Border::~Border()
{
}

std::pair<char, int> Border::Figure()
{
	if (this->getPosition() == std::make_pair(12, 14))
	{
		return std::make_pair('A', 31);//red door
	}
	else
	{
		return std::make_pair('#', 37);//white
	}
}
