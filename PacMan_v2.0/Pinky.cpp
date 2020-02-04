#include "Pinky.h"



Pinky::Pinky()
{
}


Pinky::~Pinky()
{
}

std::pair<char, int> Pinky::Figure()
{
	if (this->getStatus() != 2)
	{
		return std::make_pair('@', 45);//magenta
	}
	else
	{
		return std::make_pair('@', 44);//blue
	}
}

void Pinky::setTarget(Pacman * pac)
{
	int dir = pac->getDirection();
	pacmanTarget = pac->getPosition();
	int x, y;

	switch (dir)
	{
	case 0:
		x = pacmanTarget.first - 4;
		y = pacmanTarget.second;
		if (x < 0)
			x = 0;
		break;
	case 1:
		x = pacmanTarget.first + 4;
		y = pacmanTarget.second;
		if (x > static_cast<int>(land_.size()) - 1)
			x = static_cast<int>(land_.size()) - 1;
		break;
	case 2:
		x = pacmanTarget.first;
		y = pacmanTarget.second - 4;
		if (y < 0)
			y = 0;
		break;
	default:
		x = pacmanTarget.first;
		y = pacmanTarget.second + 4;
		if (y > static_cast<int>(land_[x].size()) - 1)
			y = static_cast<int>(land_[x].size()) - 1;
		break;
	}

	pacmanTarget = std::make_pair(x, y);
}

std::pair<int, int> Pinky::getTarget() const
{
	return pacmanTarget;
}
