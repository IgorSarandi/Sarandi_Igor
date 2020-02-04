#pragma once
#include "MotionlessObjects.h"
class Coins :
	public MotionlessObjects
{
public:
	Coins();
	~Coins();

	std::pair<char, int> Figure() override;
};

