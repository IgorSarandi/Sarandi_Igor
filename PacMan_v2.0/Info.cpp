#include <climits>
#include "Info.h"



Info::Info(unsigned int am, unsigned char curlvl, unsigned int totc, unsigned int remc, unsigned int remb) :
	AMOUNT(am), currentlevel(curlvl), totalcoins(totc), remaincoins(remc), remainboosters(remb), MAX_LEVEL(UCHAR_MAX), checklvl_(false), score_(0)
{
	this->setHealth(3);
}


Info::~Info()
{
}

void Info::addFruit(std::pair<char, int> fruit)
{
	if (static_cast<int>(fruits.size()) >= AMOUNT)
	{
		fruits.pop_back();
	}
	fruits.insert(0, 1, fruit.first);
}

std::string Info::getLastFruits() const
{
	return fruits;
}


std::string Info::getHealth()
{
	char ch = 'V';
	std::string s;
	if (health_ > 0)
	{
		for (int i = 0; i < static_cast<int>(health_); i++)
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
	else
	{
		return "";
	}
	return s;
}

void Info::setHealth(const unsigned int hp)
{
	health_ = hp;
}

void Info::Eat(const int mass)
{
	score_ += mass;
}

int Info::getScore()
{
	return score_;
}

void Info::DecreaseHP()
{
	std::string s = this->getHealth();
	int hp = 0;
	for (int i = 0; i < static_cast<int>(s.size()); i++)
	{
		if (s[i] == 'V')
		{
			hp++;
		}
	}
	this->setHealth(hp - 1);
}

bool Info::getCheckLevel()
{
	return checklvl_;
}

void Info::setCheckLevel(const bool coins, const bool booster)
{
	checklvl_ = coins && booster ? true : false;
}
