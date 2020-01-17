#include "BaseController.h"
#include <iostream>


BaseController::BaseController(int xSize, int ySize) : X_SIZE(xSize), Y_SIZE(ySize),
	colors{ {"GREEN", 10}, {"RED", 12}, {"WHITE", 15} }
{
	hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
	bufferSize_ = { static_cast<SHORT>(X_SIZE + 1), static_cast<SHORT>(Y_SIZE + 10) };
	src_ = { 0, 0, bufferSize_.X - 1,bufferSize_.Y - 1 };

	info_ = new Info(8, 0, 0, 0, 0);

	
}


BaseController::~BaseController()
{
}

std::string BaseController::Input()
{
	if (GetAsyncKeyState(87) //is W
		|| GetAsyncKeyState(119) // is w
		|| GetAsyncKeyState(38)) // is ^
		return "UP";
	else if (GetAsyncKeyState(65) // is A
		|| GetAsyncKeyState(97) // is a
		|| GetAsyncKeyState(37)) // is <
		return "LEFT";
	else if (GetAsyncKeyState(83) // is S
		|| GetAsyncKeyState(115) // is s
		|| GetAsyncKeyState(40)) // is v
		return "DOWN";
	else if (GetAsyncKeyState(68) // is D
		|| GetAsyncKeyState(100) // is d
		|| GetAsyncKeyState(39)) // is >
		return "RIGHT";
	else if (GetAsyncKeyState(VK_ESCAPE)) // is Esc
		return "END";
	else
		return "NULL";
}



void BaseController::Draw(std::vector<std::vector<std::pair<char, int>>>& objects)
{
	SetConsoleWindowInfo(hConsole_, true, &src_);
	SetConsoleScreenBufferSize(hConsole_, bufferSize_);
	SetConsoleTitle("Pac-man");
	bufferSize_.X = 0;
	bufferSize_.Y = 0;
	SetConsoleCursorPosition(hConsole_, bufferSize_);

	SetConsoleTextAttribute(hConsole_, colors.at("GREEN"));
	std::cout << info_->remainboosters << " boosters left" << std::endl;
	SetConsoleTextAttribute(hConsole_, colors.at("WHITE"));
	if (info_->remaincoins >= 100)
		std::cout << info_->remaincoins << " coins left" << std::endl;
	else
		std::cout << info_->remaincoins << " coins left " << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < static_cast<int>(objects.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(objects[i].size()); j++)
		{
			SetConsoleTextAttribute(hConsole_, objects[i][j].second);
			std::cout << objects[i][j].first;
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole_, colors.at("WHITE"));
	std::cout << "Level: ";
	SetConsoleTextAttribute(hConsole_, colors.at("RED"));
	std::cout << static_cast<int>(info_->currentlevel);
	SetConsoleTextAttribute(hConsole_, colors.at("GREEN"));
	std::cout << "\t" << info_->getLastFruits();

	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole_, colors.at("WHITE"));
	std::cout << "Lives: ";
	SetConsoleTextAttribute(hConsole_, colors.at("RED"));
	std::cout << info_->getHealth();

	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole_, colors.at("WHITE"));
	std::cout << "Score: ";
	SetConsoleTextAttribute(hConsole_, colors.at("RED"));
	std::cout << info_->getScore();
}
