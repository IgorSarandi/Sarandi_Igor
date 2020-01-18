#include "Blinky.h"
#include "Pacman.h"



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
	//if (this->getStatus() == 0)
	//{
	//	return std::make_pair(char(2), 10);//CHASE
	//}
	//else if (this->getStatus() == 1)
	//{
	//	return std::make_pair(char(2), 12);//SCATTER
	//}
	//else
	//{
	//	return std::make_pair(char(2), 15);//FRIGHTENED
	//}
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

bool Blinky::StartCondition(Info * info)
{
	return true;
}

void Blinky::setTarget(Pacman* pac)
{
	pacmanTarget = pac->getPosition();
}

std::pair<int, int> Blinky::getTarget() const
{
	return pacmanTarget;
}

