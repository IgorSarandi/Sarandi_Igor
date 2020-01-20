#pragma once
#include "Ghosts.h"
class Clyde :
	public Ghosts
{
public:
	Clyde();
	~Clyde(); 
	
	std::pair<char, int> Figure() override;

	void setCoins(const Info & info);

	bool StartCondition(const Info&) override;

	void setTarget(Pacman*) override;

	std::pair<int, int> getTarget() const override;

private:
	int startcoins_;
	int coins_;
};

