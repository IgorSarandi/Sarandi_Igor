#pragma once
#include "MotionlessObjects.h"
class Booster:
	public MotionlessObjects
{
public:
	Booster();
	~Booster();

	std::pair<char, int> Figure() override;
};

