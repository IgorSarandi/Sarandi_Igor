#include "Inky.h"



Inky::Inky(const Blinky& blinky) : blinky_(blinky)
{
}


Inky::~Inky()
{
}

std::pair<char, int> Inky::Figure()
{
	if (this->getStatus() != 2)
	{
		return std::make_pair(char(2), 75);
	}
	else
	{
		return std::make_pair(char(2), 145);
	}
}

void Inky::setCoins(const Info& info)
{
	startcoins_ = static_cast<int>(info.remaincoins);
	coins_ = startcoins_ - 30;
}

bool Inky::StartCondition(const Info& info)
{
	startcoins_ = static_cast<int>(info.remaincoins);
	return startcoins_ < coins_ ? true : false;
}

void Inky::setTarget(Pacman* pac)
{
	int dir = pac->getDirection();
	pacmanTarget = pac->getPosition();
	std::pair<int, int> gh_pos = blinky_.getPosition();
	int x, y;

	switch (dir)
	{
	case 0:
		x = pacmanTarget.first - 2;
		y = pacmanTarget.second;
		if (x < 0)
			x = 0;
		break;
	case 1:
		x = pacmanTarget.first + 2;
		y = pacmanTarget.second;
		if (x > static_cast<int>(land_.size()) - 1)
			x = static_cast<int>(land_.size()) - 1;
		break;
	case 2:
		x = pacmanTarget.first;
		y = pacmanTarget.second - 2;
		if (y < 0)
			y = 0;
		break;
	default:
		x = pacmanTarget.first;
		y = pacmanTarget.second + 2;
		if (y > static_cast<int>(land_[x].size()) - 1)
			y = static_cast<int>(land_[x].size()) - 1;
		break;
	}

	int vecX = (gh_pos.first - x) * 2;
	int vecY = (gh_pos.second - y) * 2;
	x += vecX;
	y += vecY;

	if (x < 0) 
		x = 0;
	else if (x > static_cast<int>(land_.size()) - 1)
		x = static_cast<int>(land_.size()) - 1;
	if (y < 0)
		y = 0;
	else if (y > static_cast<int>(land_[x].size()) - 1)
		y = static_cast<int>(land_[x].size()) - 1;

	pacmanTarget = std::make_pair(x, y);
}

std::pair<int, int> Inky::getTarget() const
{
	return pacmanTarget;
}