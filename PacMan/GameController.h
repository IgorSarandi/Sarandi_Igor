#pragma once
#include <string>

class GameController
{
public:
	GameController();
	~GameController();

	void AddFruit(std::pair<char, int>);

	std::string GetLastFruits() const;

private:
	std::string fruits_;
	const int amount = 8;
};

