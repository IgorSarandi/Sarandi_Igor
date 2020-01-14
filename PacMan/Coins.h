#pragma once
#include "Objects.h"
class Coins :
	public Objects
{
public:
	Coins();
	~Coins();

	std::pair<char, int> Figure() override;
};

