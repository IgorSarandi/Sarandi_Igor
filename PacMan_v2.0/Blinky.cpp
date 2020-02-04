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
		return std::make_pair('@', 41);//red
	}
	else
	{
		return std::make_pair('@', 44);//blue
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

