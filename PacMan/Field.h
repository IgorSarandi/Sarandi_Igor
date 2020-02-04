#pragma once
#include "MotionlessObjects.h"
class Field :
	public MotionlessObjects
{
public:
	Field();
	~Field();

	std::pair<char, int> Figure() override;
};

