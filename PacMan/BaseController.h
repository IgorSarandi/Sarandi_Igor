#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <map>

#include "Info.h"

class BaseController
{
public:

	BaseController(const int X, const int Y);
	virtual ~BaseController();

	int Input();

	virtual void Run() = 0;	//run game loop

	void Draw(std::vector<std::vector<std::pair<char, int>>>&); //draw game window

	//map size
	const int X_SIZE;
	const int Y_SIZE; 

	const std::map<std::string, int> colors;

	Info* info;

private:
	HANDLE  hConsole_;
	COORD bufferSize_;
	SMALL_RECT src_;
};

