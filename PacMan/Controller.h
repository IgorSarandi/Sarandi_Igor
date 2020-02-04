#pragma once
#include "BaseController.h"
#include "CreateMap.h"
#include"Objects.h"
#include "Border.h"
#include "Coins.h"
#include "Field.h"
#include "Pacman.h"
#include "Booster.h"
#include "Fruits.h"
#include "Ghosts.h"

#include <vector>
#include <string>

class Controller : 
	public BaseController
{
public:
	Controller(int, int);
	~Controller();

	CreateMap* crMap;

	void Start();

	void CheckAlive();

	void Logic();

	void Run() override;

	void GlobalCleaning();

private:
	 std::vector<std::vector<std::pair<char, int>>> gameObjects_;

	std::vector<Border*> _borders;//0
	std::vector<Coins*> _coins;//1
	std::vector<Field*> _fields;//2
	std::vector<Pacman*> _pacman;//3
	std::vector<Booster*> _booster;//4
	std::vector<Fruits*> _fruits;//5
	std::vector<Ghosts*> _ghosts;//6

	void PacmanUpdate(int, int, int, int, int);
	void PacmanActions();
	void GhostsActions();
	void UpdateFruits();
	int key = INT_MAX;
};

