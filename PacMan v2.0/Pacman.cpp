#include "Pacman.h"
#include "CreateMap.h"



Pacman::Pacman()
{
}


Pacman::~Pacman()
{
}

std::pair<char, int> Pacman::Figure()
{
	return std::make_pair('0', 43);//yellow
}

bool Pacman::CheckDirection()
{
	int x = this->getPosition().first;
	int y = this->getPosition().second;
	int dir = this->getDirection();

	if (&land_[x - 1][y] != 0 && dir != 0)
	{
		return true;
	}
	if (&land_[x + 1][y] != 0 && dir != 1)
	{
		return true;
	}
	if (&land_[x][y - 1] != 0 && dir != 2)
	{
		return true;
	}
	if (&land_[x][y + 1] != 0 && dir != 3)
	{
		return true;
	}
	return false;
}
