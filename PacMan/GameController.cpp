#include "GameController.h"



GameController::GameController()
{
}


GameController::~GameController()
{
}

void GameController::AddFruit(std::pair<char, int> fruit)
{
	if (fruits_.size() >= amount)
	{
		fruits_.pop_back();
	}
	fruits_.insert(0, 1, fruit.first);
}

std::string GameController::GetLastFruits() const
{
	return fruits_;
}

