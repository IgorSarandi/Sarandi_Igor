#pragma once
#include "Ghosts.h"
class Blinky :
	public Ghosts
{
public:
	Blinky();
	~Blinky();

	std::pair<char, int> Figure() override;

	void Character() override;
};

