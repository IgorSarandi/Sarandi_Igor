#pragma once
#include "Ghosts.h"
class Pinky :
	public Ghosts
{
public:
	Pinky();
	~Pinky();

	std::pair<char, int> Figure() override;

	void Character() override;

	bool StartCondition(Info*) override;

	void setTarget(Pacman*) override;

	std::pair<int, int> getTarget() const override;
};

