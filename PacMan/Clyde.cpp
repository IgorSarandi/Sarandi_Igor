#include "Clyde.h"
#include <math.h>



Clyde::Clyde()
{
}


Clyde::~Clyde()
{
}

std::pair<char, int> Clyde::Figure()
{
	if (this->getStatus() != 2)
	{
		return std::make_pair(char(2), 78);
	}
	else
	{
		return std::make_pair(char(2), 145);
	}
}

void Clyde::setCoins(const Info & info)
{
	startcoins_ = static_cast<int>(info.remaincoins);
	coins_ = static_cast<int>((startcoins_ / 3) * 2);
}

bool Clyde::StartCondition(const Info & info)
{
	startcoins_ = static_cast<int>(info.remaincoins);
	return startcoins_ < coins_ ? true : false;
}

void Clyde::setTarget(Pacman * pac)
{
	pacmanTarget = pac->getPosition();
	std::pair<int, int> gh_pos = this->getPosition();
	double pow1, pow2, dist;


	pow1 = pow(gh_pos.first - pacmanTarget.first, 2);
	pow2 = pow(gh_pos.second - pacmanTarget.second, 2);
	dist = sqrt(pow1 + pow2);

	if (dist > 8)
	{
		pacmanTarget = pac->getPosition();
	}
	else
	{
		pacmanTarget = this->getTargetField();
	}
}

std::pair<int, int> Clyde::getTarget() const
{
	return pacmanTarget;
}
