#include "BaseController.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


BaseController::BaseController(const int xSize, const int ySize) : X_SIZE(xSize), Y_SIZE(ySize),
	colors{ {"GREEN", 32}, {"RED", 31}, {"WHITE", 37} }, old_key(INT32_MAX)
{
    std::cout << "\033[8;43;132t";// set terminal height to 43, width to 132

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

void BaseController::Input()
{
    if (kbhit_())
    {
        char ch = getch_();

            //UP
        if (ch == 'W' || ch == 'w')
            old_key = 0;

            //DOWN
        else if (ch == 'S' || ch == 's')
            old_key = 1;

            //LEFT
        else if (ch == 'A' || ch == 'a')
            old_key = 2;

            //RIGHT
        else if (ch == 'D' || ch == 'd')
            old_key = 3;


        else if (ch == '\033') // Esc sequence
        {
            getch_();
            ch = getch_();
            if (ch == 'A') // UP
                old_key = 0;
            else if (ch == 'B') // DOWN
                old_key = 1;
            else if (ch == 'C') // RIGHT
                old_key = 3;
            else if (ch == 'D') // LEFT
                old_key = 2;
            else // Esc
            {
                std::cout << "ESC";
                reset_();
                old_key = -1;
            }
        }
    }
    else
    {
        old_key = INT32_MAX;
    }
}


void BaseController::Draw(std::vector<std::vector<std::pair<char, int>>>& objects)
{
    usleep(100000);
	home();
    set_display_atrib(colors.at("GREEN"));
	std::cout << info->remainboosters << " boosters left";
	gotoxy(1,2);
    set_display_atrib(colors.at("WHITE"));
	if (info->remaincoins >= 100)
		std::cout << info->remaincoins << " coins left";
	else
		std::cout << info->remaincoins << " coins left ";


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
    _y += static_cast<int>(objects.size());

    gotoxy(_x, _y++);
    set_display_atrib(colors.at("WHITE"));
    std::cout << "Level: ";
    set_display_atrib(colors.at("RED"));
    std::cout << static_cast<int>(info->currentlevel);
    set_display_atrib(colors.at("GREEN"));
    std::cout << "\t" << info->getLastFruits();

    gotoxy(_x, _y++);
    set_display_atrib(colors.at("WHITE"));
    std::cout << "Lives: ";
    set_display_atrib(colors.at("RED"));
    std::cout << info->getHealth();

    gotoxy(_x, _y);
    set_display_atrib(colors.at("WHITE"));
    std::cout << "Score: ";
    set_display_atrib(colors.at("RED"));
    std::cout << info->getScore();

    fflush(stdout);
}
