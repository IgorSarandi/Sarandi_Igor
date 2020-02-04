#pragma once
#include "Ghosts.h"
#include "Info.h"
#include "Pacman.h"

class Pinky :
	public Ghosts
{
public:
	Pinky();
	~Pinky();

	std::pair<char, int> Figure() override;

	void setTarget(Pacman*) override;

	std::pair<int, int> getTarget() const override;
};

