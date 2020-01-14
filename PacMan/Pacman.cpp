#include "Pacman.h"

int Pacman::score_ = 0;
int Pacman::health_ = 0;
int Pacman::speed_ = 0;
bool Pacman::checklvl_ = false;
bool Pacman::energy_ = false;
std::pair<int, int> Pacman::startposition_ = std::make_pair(23, 14);

Pacman::~Pacman()
{
}


Pacman& Pacman::getSingleton()
{
	static Pacman singleton;
	singleton.SetStartPosition(23, 14);
	singleton.SetPosition(23, 14);
	singleton.SetHealth(3);
	singleton.SetEnergy(false);
	return singleton;
}

void Pacman::Eat(const int mass)
{
	score_ += mass;
}

std::pair<char, int> Pacman::Figure()
{
	return std::make_pair('O', 78);
}

int Pacman::GetScore()
{
	return score_;
}

std::string Pacman::GetHealth()
{
	char ch = char(3);
	std::string s;
	if (health_ > 0)
	{
		for (int i = 0; i < health_; i++)
		{
			s.push_back(ch);
			s.push_back(' ');
		}
		int t = 3 - health_;
		if (t > 0)
		{
			s.push_back(' ');
			s.push_back(' ');
		}
	}
	return s;
}

void Pacman::SetHealth(const int hp)
{
	health_ = hp;
}

int Pacman::GetSpeed()
{
	return speed_;
}

void Pacman::SetSpeed(const int speed)
{
	speed_ = speed;
}

bool Pacman::GetCheckLevel()
{
	return checklvl_;
}

void Pacman::SetCheckLevel(const bool coins, const bool booster)
{
	checklvl_ = coins && booster ? true : false;
}

bool Pacman::GetEnergy()
{
	return energy_;
}

void Pacman::SetEnergy(const bool energy)
{
	energy_ = energy;
}

std::pair<int, int> Pacman::GetStartPosition()
{
	return startposition_;
}

void Pacman::SetStartPosition(int x, int y)
{
	startposition_ = std::make_pair(23, 14);
}
