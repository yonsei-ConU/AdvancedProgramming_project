
#define background_closed_color 80,80,80
#define background_opened_color 40,40,40
#define background_flag_color 20,20,20
#define background_aim_color 140,90,90
#define background_incorrect_color 150,40,40
#define flag_color 255,255,255
#define color_1 210,0,0
#define color_2 0,0,230
#define color_3 0,200,0
#define color_4 255,0,255
#define color_5 255,255,0
#define color_6 0,255,255
#define color_init 0,0,0

#define sample_1 "   1  2  3  4\n \
1)\033[48;2;80;80;80m @  @  @  @ \033[0m(1   다음과 같은 경우에서 3 3을 입력할 경우\n \
2)\033[48;2;80;80;80m @  @  @  @ \033[0m(2   (3,3)주변에 깃발이 2개 존재하므로\n \
3)\033[48;2;80;80;80m @  @ \033[48;2;40;40;40m\033[38;2;0;0;230m 2  \033[38;2;255;255;255m! \033[0m(3   인접하고 있는 열리지 않은 모든 칸이 열립니다\n \
4)\033[48;2;80;80;80m @  @ \033[48;2;40;40;40m\033[38;2;0;0;230m 2  \033[38;2;255;255;255m! \033[0m(4   이때 깃발을 잘못 설치해 놓았으면 지뢰 칸이 열려 바로 게임 오버가 됩니다\n \
   1  2  3  4\n\n"

/*
* @brief 글자 색을 바꾸는 함수 (0,0,0 입력 시 기본값으로 초기화)
* @param r 빨간색(0-255)
* @param g 초록색(0-255)
* @param b 파란색(0-255)
* @return 없음
*/
void font_color(int r, int g, int b) {
	if(!r && !g && !b) {
		printf("\033[0m");
		return;
	}
	printf("\033[38;2;%d;%d;%dm", r, g, b);
}

/*
* @brief 배경 색을 바꾸는 함수 (0,0,0 입력 시 기본값으로 초기화)
* @param r 빨간색(0-255)
* @param g 초록색(0-255)
* @param b 파란색(0-255)
* @return 없음
*/
void background_color(int r, int g, int b) {
		if(!r && !g && !b) {
		printf("\033[0m");
		return;
	}
	printf("\033[48;2;%d;%d;%dm", r, g, b);
}