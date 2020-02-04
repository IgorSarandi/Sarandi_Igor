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
		return std::make_pair(char(30), 4);//тусклый
	}
	else
	{
		return std::make_pair('#', 79);
	}
}
