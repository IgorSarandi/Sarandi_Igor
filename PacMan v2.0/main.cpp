#include "Controller.h"



int main()
{
    printf("\033[2J");//clear screen
	Controller cr(31, 28);
	cr.Run();
}