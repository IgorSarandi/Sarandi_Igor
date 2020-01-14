#pragma once
#include "Objects.h"
class Fruits :
	public Objects
{
public:
	Fruits();
	~Fruits();

	int CalculateScore(int);

	std::pair<char, int> Figure() override;
private:
	char picture_;
};

