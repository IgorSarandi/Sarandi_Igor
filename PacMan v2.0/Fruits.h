#pragma once
#include "MotionlessObjects.h"
class Fruits :
	public MotionlessObjects
{
public:
	Fruits();
	~Fruits();

	int CalculateScore(int);

	std::pair<char, int> Figure() override;
private:
	char picture_;
};

