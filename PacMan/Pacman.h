#pragma once
#include "Objects.h"
#include <string>
#include <memory>

class Pacman :
	public Objects
{
public:
	Pacman(const Pacman&) = delete;

	Pacman& operator=(const Pacman&) = delete;

	~Pacman();

	std::pair<char, int> Figure() override;

	static Pacman& getSingleton();

	static int GetScore();

	static void Eat(const int);


	static std::string GetHealth();

	static void SetHealth(const int);


	static int GetSpeed();

	static void SetSpeed(const int);


	static bool GetCheckLevel();

	static void SetCheckLevel(const bool = false, const bool = false);


	static bool GetEnergy();

	static void SetEnergy(const bool);

	static std::pair<int, int> GetStartPosition(); //get start object position

	static void SetStartPosition(int _x, int _y); //set start object position

private:
	Pacman() {};

	static int score_;
	static int health_;
	static int speed_;
	static bool checklvl_;
	static bool energy_;
	static std::pair<int, int> startposition_;
};

