#pragma once
#include "MotionlessObjects.h"
class Border :
	public MotionlessObjects
{
public:
	Border();
	~Border();

	std::pair<char, int> Figure() override;
};

