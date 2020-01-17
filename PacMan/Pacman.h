#pragma once
#include "MovingObjects.h"
class Pacman :
	public MovingObjects
{
public:
	Pacman();
	~Pacman();

	std::pair<char, int> Figure() override;

	bool CheckDirection();
};

