#pragma once
#include "Ghosts.h"
#include "Info.h"

class Blinky :
	public Ghosts
{
public:
	Blinky();
	~Blinky();

	std::pair<char, int> Figure() override;

	void Character() override;

	bool StartCondition(Info*) override;

	void setTarget(Pacman*) override;

	std::pair<int, int> getTarget() const override;
};

