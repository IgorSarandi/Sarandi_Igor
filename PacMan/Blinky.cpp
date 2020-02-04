#include "Blinky.h"



Blinky::Blinky()
{
}


Blinky::~Blinky()
{
}

std::pair<char, int> Blinky::Figure()
{
	if (this->getStatus() != 2)
	{
		return std::make_pair(char(2), 76);
	}
	else
	{
		return std::make_pair(char(2), 145);
	}
}

void Blinky::Character()
{
	if (!this->getDoorPassed())
	{
		if (this->getPosition().first < this->doorCoord_.first)
		{
			this->setDoorPassed(true);
		}
	}
}

void Blinky::setTarget(Pacman* pac)
{
	pacmanTarget = pac->getPosition();
}

std::pair<int, int> Blinky::getTarget() const
{
	return pacmanTarget;
}

