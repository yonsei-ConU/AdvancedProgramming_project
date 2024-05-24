#include <Windows.h>

enum ColorType{
BLACK,  	//0
DarkBlue,	//1
DarkGreen,	//2
DarkSkyBlue,//3
DarkRed,  	//4
DarkPurple,	//5
DarkYellow,	//6
GRAY,		//7
DarkGray,	//8
BLUE,		//9
GREEN,		//10
SkyBlue,	//11
RED,		//12
PURPLE,		//13
YELLOW,		//14
WHITE		//15
} COLOR;

void color(int font_color, int background_color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), font_color + (background_color << 4));
}