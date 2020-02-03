#include "BaseController.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


BaseController::BaseController(const int xSize, const int ySize) : X_SIZE(xSize), Y_SIZE(ySize),
	colors{ {"GREEN", 32}, {"RED", 31}, {"WHITE", 37} }
{
    //sizeX_ = X_SIZE - 2;
    //sizeY_ = Y_SIZE + 10;

	info = new Info(8, 1, 0, 0, 0);
    tcgetattr(STDIN_FILENO, &old); /* grab old terminal i/o settings */
    current = old; /* make new settings same as old settings */
    current.c_lflag &= ~ICANON; /* disable buffered i/o */
    current.c_lflag &= ~ECHO; /* set no echo mode */
    tcsetattr(STDIN_FILENO, TCSANOW, &current); /* use these new terminal i/o settings now */
}


BaseController::~BaseController()
{
}

void BaseController::reset_()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

bool BaseController::kbhit_()
{
    int ch;
    int oldf;

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

char BaseController::getch_()
{
    char ch;
    ch = getchar();
    return ch;
}

int BaseController::Input()
{
    char ch;
    if (kbhit_())
    {
        ch = getch_();

        //UP
        if(ch == 'W' || ch == 'w')
            return 0;

        //DOWN
        else if (ch == 'S' || ch == 's')
            return 1;

        //LEFT
        else if (ch == 'A' || ch == 'a')
            return 2;

        //RIGHT
        else if (ch == 'D' || ch == 'd')
            return 3;

        //END
        else if (ch == '\033') // is Esc sequence
        {
            getch_();
            ch = getch_();
            if(ch == 'A') // is UP
                return 0;
            else if (ch == 'B') // is DOWN
                return 1;
            else if (ch == 'C') // is RIGHT
                return 3;
            else if(ch == 'D') // is LEFT
                return 2;
            else // is Esc
            {
                std::cout << "ESC";
                reset_();
            }
        }
            return -1;
    }

	//NULL
		return INT32_MAX;
}


void BaseController::Draw(std::vector<std::vector<std::pair<char, int>>>& objects)
{
	home();
    set_display_atrib(colors.at("GREEN"));
	std::cout << info->remainboosters << " boosters left";// << std::endl;
	gotoxy(1,2);
    set_display_atrib(colors.at("WHITE"));
	if (info->remaincoins >= 100)
		std::cout << info->remaincoins << " coins left";// << std::endl;
	else
		std::cout << info->remaincoins << " coins left ";// << std::endl;
	std::cout << std::endl;
    int _x = 1;
    int _y = 4;

    for(int i = 0; i < static_cast<int>(objects.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(objects[i].size()); j++)
        {
            gotoxy(_x + j, _y + i);
            set_display_atrib(objects[i][j].second);
            std::cout << objects[i][j].first;
            resetcolor();
        }
    }

    gotoxy(_x, _y + static_cast<int>(objects.size()));

    std::cout << std::endl;
    set_display_atrib(colors.at("WHITE"));
    std::cout << "Level: ";
    set_display_atrib(colors.at("RED"));
    std::cout << static_cast<int>(info->currentlevel);
    set_display_atrib(colors.at("GREEN"));
    std::cout << "\t" << info->getLastFruits();

    std::cout << std::endl;
    set_display_atrib(colors.at("WHITE"));
    std::cout << "Lives: ";
    set_display_atrib(colors.at("RED"));
    std::cout << info->getHealth();

    std::cout << std::endl;
    set_display_atrib(colors.at("WHITE"));
    std::cout << "Score: ";
    set_display_atrib(colors.at("RED"));
    std::cout << info->getScore();

    fflush(stdout);
}
