#pragma once
#include <string>
#include <vector>
#include <map>
#include <termios.h>

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
protected:
    inline void clrscr() {printf("\033[2J");} //clear the screen, move to (1,1)
    void reset_();
private:
    inline void home() {printf("\033[H");} //Move cursor to the indicated row, column (origin at 1,1)
    inline void gotoxy(int x, int y) {printf("\033[%d;%dH", y, x);}
    inline void resetcolor() {printf("\033[0m");}
    inline void set_display_atrib(int color) {printf("\033[%dm",color);}

    struct termios old, current;
    bool kbhit_();
    char getch_();
};

