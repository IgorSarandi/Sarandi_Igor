#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <map>

#include "Info.h"

class BaseController
{
public:

	BaseController(int, int);//params is size by vertical and horisontal
	virtual ~BaseController();

	std::string Input();

	virtual void Run() = 0;	//run game loop

	void Draw(std::vector<std::vector<std::pair<char, int>>>&); //draw game window

	//map size
	const int X_SIZE;
	const int Y_SIZE; 

	const std::map<std::string, int> colors;

protected:
	Info* info_;

private:
	HANDLE  hConsole_;
	COORD bufferSize_;
	SMALL_RECT src_;
};

