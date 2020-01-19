#pragma once
#include "Ghosts.h"
#include "Info.h"
#include "Pacman.h"
#include "Blinky.h"
#include "Info.h"

class Inky :
	public Ghosts
{
public:
	Inky(const Blinky&);
	~Inky(); 
	
	std::pair<char, int> Figure() override;

	void setCoins(const Info & info);

	bool StartCondition(const Info&) override;

	void setTarget(Pacman*) override;

	std::pair<int, int> getTarget() const override;

private:
	const Blinky & blinky_;
	int startcoins_;
	int coins_;
};

