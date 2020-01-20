#include "BaseController.h"
#include <iostream>


BaseController::BaseController(const int xSize, const int ySize) : X_SIZE(xSize), Y_SIZE(ySize),
	colors{ {"GREEN", 10}, {"RED", 12}, {"WHITE", 15} }
{
	hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
	bufferSize_ = { static_cast<SHORT>(X_SIZE - 2), static_cast<SHORT>(Y_SIZE + 10) };//+1
	src_ = { 0, 0, bufferSize_.X - 1,bufferSize_.Y - 1 };

	info = new Info(8, 1, 0, 0, 0);

	
}


BaseController::~BaseController()
{
}

int BaseController::Input()
{
	//UP
	if (GetAsyncKeyState(87) //is W
		|| GetAsyncKeyState(119) // is w
		|| GetAsyncKeyState(38)) // is ^
		return 0;
	//DOWN
	else if (GetAsyncKeyState(83) // is S
		|| GetAsyncKeyState(115) // is s
		|| GetAsyncKeyState(40)) // is v
		return 1;
	//LEFT
	else if (GetAsyncKeyState(65) // is A
		|| GetAsyncKeyState(97) // is a
		|| GetAsyncKeyState(37)) // is <
		return 2;
	//Right
	else if (GetAsyncKeyState(68) // is D
		|| GetAsyncKeyState(100) // is d
		|| GetAsyncKeyState(39)) // is >
		return 3;
	//END
	else if (GetAsyncKeyState(VK_ESCAPE)) // is Esc
		return -1;
	//NULL
	else
		return INT_MAX;
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
	std::cout << info->remainboosters << " boosters left" << std::endl;
	SetConsoleTextAttribute(hConsole_, colors.at("WHITE"));
	if (info->remaincoins >= 100)
		std::cout << info->remaincoins << " coins left" << std::endl;
	else
		std::cout << info->remaincoins << " coins left " << std::endl;
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
	std::cout << static_cast<int>(info->currentlevel);
	SetConsoleTextAttribute(hConsole_, colors.at("GREEN"));
	std::cout << "\t" << info->getLastFruits();

	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole_, colors.at("WHITE"));
	std::cout << "Lives: ";
	SetConsoleTextAttribute(hConsole_, colors.at("RED"));
	std::cout << info->getHealth();

	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole_, colors.at("WHITE"));
	std::cout << "Score: ";
	SetConsoleTextAttribute(hConsole_, colors.at("RED"));
	std::cout << info->getScore();
}
