#pragma once
#include "Objects.h"
class Booster:
	public Objects
{
public:
	Booster();
	~Booster();

	std::pair<char, int> Figure() override;
};

