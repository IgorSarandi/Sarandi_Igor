#include "Fruits.h"
#include <random>
#include <climits>


Fruits::Fruits()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(0, 3);
	int num = dis(gen);

	if (num == 0)
		picture_ = '$';
	else if (num == 1)
		picture_ = '%';
	else if (num == 2)
		picture_ = '&';
	else if (num == 3)
		picture_ = '*';
}


Fruits::~Fruits()
{
}

int Fruits::CalculateScore(int lvl)
{
	int min = 100;
	int max = 5000;
	int div = (max - min) / UCHAR_MAX;
	div = div * (lvl - 1);

	return min + div;
}

std::pair<char, int> Fruits::Figure()
{
	return std::make_pair(picture_, 32);//green
}
