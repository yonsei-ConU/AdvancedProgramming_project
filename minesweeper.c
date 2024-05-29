#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "color.h"

// 컴파일 시 color.h 파일을 같은 폴더에 놓고 진행할 것

#define mine '#'
#define closed '@'
#define opened ' '
#define flag '!'

#define bool int
#define true 1
#define false 0

int n, m, c = 0;        // n은 가로, m은 세로, c는 지뢰 개수
int revealed = 0;       // 1개의 칸 열릴때마다 1 증가
int flaged = 0;         // 깃발 1개 꽂을때마다 1 증가
bool game_over = false;      // 지뢰 칸 열면 1로 변경
int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

typedef struct{ 
    bool is_mine;        // 기본값 0, 지뢰일 경우 1
    bool is_flag;        // 기본값 0, 깃발 세워졌을 경우 1
    bool is_open;        // 기본값 0, 열렸을 경우 1
    int adj;            // 인접한 칸의 지뢰 개수
} Board;    

Board board[20][20];    //board[y좌표][x좌표]
/**
 * @brief 초기화 담당 함수. 크기와 지뢰 개수를 입력받고 board를 초기화함
 * @return 없음
 */
void init();
/**
 * @brief board를 출력하는 함수.
 * @return 없음
 */
void print_board();     // 배열 관점에서의 좌표 (0부터 시작)
/**
 * @brief 지뢰의 위치를 정하고 각 자리의 adj(주변 지뢰의 개수)를 계산하는 함수.
 * @param x 시작지점의 x좌표 (0부터 시작)
 * @param y 시작지점의 y좌표 (0부터 시작)
 * @return 없음
 */
void set_mine(int x, int y);        // 배열 관점에서의 좌표 (0부터 시작)
/**
 * @brief 지뢰의 칸을 여는 함수
 * @param x 열고자 하는 지점의 x좌표 (0부터 시작)
 * @param y 열고자 하는 지점의 y좌표 (0부터 시작)
 * @return 없음
 */
void reveal(int x, int y);          // 배열 관점에서의 좌표 (0부터 시작)

int main() {
    int aimX, aimY;                 // 사용자 관점에서의 좌표(1부터 시작)으로 입력은 받으나 1씩 감소시켜 0 시작(배열 관점)으로 변형

    init();
    print_board();

    printf("\nx, y 좌표를 공백으로 구분하여 입력해 주세요.\n");
    printf("처음 여는 칸과 그 주변은 지뢰가 아닙니다.\n");
    printf("범위 x(1~%d) y(1~%d) : ", n, m);
    scanf("%d %d", &aimX, &aimY);

    if(c > (n * m - 9) / 5 * 4)     // 지뢰가 전체 칸의 80% 이상이면 오래 (몇 초 이상) 걸리므로 메시지 표시
        printf("\n*지뢰의 개수가 많아 시간이 오래 걸릴 수 있습니다. 기다려 주세요\n");
    
    aimX--; aimY--;                 // 배열 관점의 좌표료 변경

    set_mine(aimX, aimY);
    reveal(aimX, aimY);

    if(n * m == c + revealed) {     // 4*4 등의 작은 판에서 시작하자마자 승리하는 경우 검사
            print_board(-1, -1);
            printf("\n시작하자마자 게임이 끝났군요!\n%d개의 지뢰를 모두 찾았습니다. 게임 승리.\n", c);
            return 0;
    }

    time_t start_time = time(NULL); // 소요 시간 측정용도로 시작 시간 저장

    while(1) {
        print_board();
        
        printf("\n(%d,%d)를 입력하였습니다. 새 좌표를 입력해 주세요 : ", aimX + 1, aimY + 1);
        scanf("%d %d", &aimX, &aimY);

        while(!(1 <= abs(aimX) && abs(aimX) <= n) || !(1 <= abs(aimY) && abs(aimY) <= m)) {
            printf("잘못된 입력입니다. 범위에 맞춰 다시 입력해 주세요.\n");
            printf("다시 입력해 주세요 : ");
            scanf("%d %d", &aimX, &aimY);
        }

        if(aimX < 0 || aimY < 0) {
            aimX = abs(aimX); aimY = abs(aimY);
            aimX--; aimY--;
            
            if(board[aimY][aimX].is_flag) {
                board[aimY][aimX].is_flag = false;
                flaged--;
            }
            else if(board[aimY][aimX].is_open == false) {
                board[aimY][aimX].is_flag = true;
                flaged++;
                }
                else printf("이미 열린 칸에 깃발을 설치할 수 없습니다.\n");
            continue;               // 깃발 설치 시에는 그 칸을 여는 것이 아니므로 통과
        }
        
        
        aimX--; aimY--;
        reveal(aimX, aimY);

        if(game_over) {
            print_board();
            printf("\n(%d,%d)은 지뢰였습니다. 게임 오버!\n", aimX + 1, aimY + 1);
            printf("소요 시간 : %d초", (int)(time(NULL) - start_time));
           return 0;
        }
        if(n * m == c + revealed) {
            print_board();
            printf("\n%d개의 지뢰를 모두 찾았습니다. 게임 승리!\n", c);
            printf("소요 시간 : %d초", (int)(time(NULL) - start_time));
            return 0;
        }
    }
}

void init() {
    printf("##지뢰 찾기##\n\n");
    printf("지뢰 찾기는 직사각형의 판 안에서 모든 지뢰를 찾아내면 승리하는 간단한 게임입니다.\n");
    printf("기본적인 조작은 좌표 입력으로 진행되며 '왼쪽 위'에서 1부터 시작합니다.\n\n");
    printf("좌표는 공백으로 구분됩니다. 칸에 적힌 숫자는 인접한 칸에 있는 지뢰의 수를 의미합니다.\n");
    printf("좌표 앞에 -를 한 번 이상 붙일 경우 (ex. -4 7 또는 -9 -2) 깃발을 설치하는 것으로 간주됩니다.\n");
    printf("깃발이 설치된 자리에 깃발을 다시 설치하면 깃발이 없어집니다.\n\n");
    printf(sample_1);
    printf("게임을 시작합니다.\n");

    while(1) {
        printf("가로 크기를 입력해 주세요 (5~20) : ");
        scanf("%d", &n);
        if(5 <= n && n <= 20) break;
        else printf("다시 입력해 주세요, ");
    }

    while(1) {
        printf("세로 크기를 입력해 주세요 (5~20) : ");
        scanf("%d", &m);
        if(5 <= m && m <= 20) break;
        else printf("다시 입력해 주세요, ");
    }

    while(1) {
        printf("\n추천 난이도 : 쉬움(%d) 보통(%d) 어려움(%d)\n",
            ((n * m - 9) / 10), ((n * m - 9) / 6), ((n * m - 9) / 4));
        printf("지뢰의 개수를 입력해 주세요 (1~%d) : ", n * m - 9);
        scanf("%d", &c);     // 초반 억까 방지용으로 자신과 주변 1칸인 9칸에는 지뢰가 들어갈 수 없기 때문에 9를 뺌
        if(1 <= c && c <= n * m - 9) break;
        else printf("다시 입력해 주세요. ");
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            board[j][i].is_flag = false;
            board[j][i].is_mine = false;
            board[j][i].is_open = false;
        }
    }
}

void print_board() {
    printf("-----------------------------------------------");
    printf("\n지뢰 수 : %d, 전체 칸 : %d, 남은 지뢰 : %d\n  ", c, n * m, c - flaged);
    for(int i = 0; i < n; i++)
        printf(" %2d", i + 1);      // 맨 위 숫자를 출력하는 역할
    printf("\n");

    for(int i = 0; i < m; i++) {
        printf("%2d)", i + 1);      // 왼쪽 숫자를 출력하는 역할

        for(int j = 0; j < n; j++) {
        if(game_over && board[i][j].is_mine) {
            if(board[i][j].is_flag == false)    // 패배 후 지뢰가 있었지만 깃발을 설치하지 못했을 때
                background_color(background_incorrect_color);
            printf(" %c ", mine);  
        }
        else {
            if(board[i][j].is_flag == true) {
                if(game_over && !board[i][j].is_mine)
                    background_color(background_incorrect_color);   // 패배 후 지뢰가 없는 자리에 깃발을 설치했을 떄
                else background_color(background_flag_color);       // 게임 중 깃발 자리
                font_color(flag_color);
                printf(" %c ", flag);
            }
            else if(board[i][j].is_open == true) {     // 게임 중 열려 있는 자리
                if(board[i][j].adj) {               // 게임 중 열려 있는 자리 (숫자 있음)
                    switch(board[i][j].adj) {
                        case 1 : font_color(color_1); break;
                        case 2 : font_color(color_2); break;
                        case 3 : font_color(color_3); break;
                        case 4 : font_color(color_4); break;
                        case 5 : font_color(color_5); break;
                        case 6 : font_color(color_6); break;
                    }
                background_color(background_opened_color);
                printf(" %d ", board[i][j].adj);
                }
                else {      // 게임 중 열려 있는 빈 자리
                    background_color(background_opened_color);
                    printf(" %c ", opened);
                }
            }
            else {          // 열리지 않은 자리
                background_color(background_closed_color);
                printf(" %c ", closed);
            }
        }
        font_color(color_init);
        }

        background_color(color_init);
        printf("(%d\n", i + 1);     // 오른쪽 숫자를 출력하는 역할
    }

    printf("  ");
    for(int i = 0; i < n; i++)
        printf(" %2d", i + 1);      // 맨 아래 숫자를 출력하는 역할
}

void set_mine(int x, int y) {
    int tempX, tempY;
    int count;

    for(count = 0; count < c; count++) {
        srand(clock() + count);                      // time(NULL) 의 경우 1초 단위로 업데이트되어 반복문에서 사용 부적합
        tempX = (rand() * rand() + count * x) % n;   // 최대한의 무작위성을 가질 수 있도록 변하는 값들을 활용
        tempY = (rand() * rand() + count * y) % m;

        if((abs(tempX - x) > 1 || abs(tempY - y) > 1) && board[tempY][tempX].is_mine == false)
            board[tempY][tempX].is_mine = true;         // 첫 칸 주위 1칸에는 지뢰를 배치하지 않도록 하여 초반 억까 방지
        else count--;
    }

    for(int i = 0; i < n; i++) {       // 지뢰를 모두 배치한 다음 각 칸마다 주변 지뢰 개수를 셈
        for(int j = 0; j < m; j++) {
            count = 0;
            for(int k = 0; k < 8; k++) {
                int nx = i + dx[k];
                int ny = j + dy[k];
                if((0 <= nx && nx < n) && (0 <= ny && ny < n)) {
                    if(board[ny][nx].is_mine == true)
                        count++;
                }
            }
            board[j][i].adj = count;
        }
    }
}

void reveal(int x, int y) {
    int count = 0;
    bool near_blank = false;

    if(board[y][x].is_flag) return;     // 깃발이 설치된 구역은 판정하지 않음

    if(board[y][x].is_open) {           // 열려 있고, adj가 있다는 것은
        if(board[y][x].adj) {           // 주변 칸을 바로 열 목적으로 숫자 칸을 입력했다는 것
           for(int i = 0; i < 8; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if((0 <= nx && nx < n) && (0 <= ny && ny < m)) {
                    if(board[ny][nx].is_flag)
                        count++;
                    if(board[ny][nx].is_open == false)
                        near_blank = true;
                }
            }                           // 주변에 깃발이 알맞은 수로 꽂아져 있고 안 열린 칸이 있으면
            if((board[y][x].adj == count) && board[y][x].adj && near_blank) {
                for(int i = 0; i < 8; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if((0 <= nx && nx < n) && (0 <= ny && ny < m)) {
                        if(board[ny][nx].is_flag == false && board[ny][nx].is_open == false)
                            reveal(nx, ny);   // 해당 칸을 모두 열기
                    }
                }
            }
        }
        return;                         // 열려 있는 경우 또 열 필요는 없으므로 종료
    }

    board[y][x].is_open = true;            // 여기서부터는 닫혀있었던 칸을 여는 경우에 실행됨
    revealed++;

    if(board[y][x].is_mine == true) {      // 사용자가 직접 지뢰 칸을 열었을 때만 실행됨.
        game_over = true;                  // 또는 깃발을 잘못 설치하고 숫자 칸을 열 때
        return;
    }                               // 재귀에서 지뢰 칸은 그 전에 해당 구문(if~adj)에서 return당하여 실행되지 않음
    if(board[y][x].adj) return;     // 지뢰 칸에 도달하기 전에 adj가 양수인 칸을 반드시 지나야 하기 때문

    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if((0 <= nx && nx < n) && (0 <= ny && ny < m)) {
            if(board[ny][nx].is_open == false && board[ny][nx].is_flag == false)    
                reveal(nx, ny);
        }
    }
}