#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "color.h"

#define mine '#'
#define closed '@'
#define opened '_'
#define flag '!'

int n, m, c = 0;        // n은 가로, m은 세로, c는 지뢰 개수
int revealed, flaged = 0;
int game_over = 0;

typedef struct{ 
    int is_mine;        // 기본값 0, 지뢰일 경우 1
    int is_flag;        // 기본값 0, 깃발 세워졌을 경우 1
    int is_open;        // 기본값 0, 열렸을 경우 1
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
 * board[20][20]으로 하면 앞의 [20]이 y좌표가 되고 뒤의 [20]이 x좌표가 됨.
 * 다른 부분에서는 관용적으로 x, y 형식으로 보고 계산을 진행하나
 * print 부분에서는 x와 y를 바꿔 사용자의 혼란을 줄임
 * @return 없음
 */
void print_board(int x, int y);     // x y 반전
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
 */
void reveal(int x, int y);          // 배열 관점에서의 좌표 (0부터 시작)

int main() {
    int aimX, aimY;                 // 사용자 관점에서의 좌표(1부터 시작)으로 입력은 받으나 1씩 감소시켜 0 시작(배열 관점)으로 변형
    int flag_input = 0;             //
    time_t start_time = time(NULL); // 소요 시간 측정용도로 시작 시간 저장

    init();
    print_board(-1, -1);

    printf("\nx, y 좌표를 공백으로 구분하여 입력해 주세요.\n");
    printf("처음 여는 칸과 그 주변은 지뢰가 아닙니다.\n");
    printf("범위 x(1~%d) y(1~%d) : ", n, m);
    scanf("%d %d", &aimX, &aimY);

    aimX--; aimY--;

    set_mine(aimX, aimY);
    reveal(aimX, aimY);
    revealed++;

    while(1) {
        print_board(aimX, aimY);
        printf("\n(%d,%d)를 입력하였습니다. 깃발을 꽂기 위해서는 좌표 앞에 -를 붙여 주세요. ex) -4 -10\n", aimX + 1, aimY + 1);
        printf("입력은 공백으로 구분됩니다 : ");
        scanf("%d %d", &aimX, &aimY);

        while(aimX * aimY <= 0) {
            printf("잘못된 입력입니다. 깃발을 꽂을 경우 둘 다 -를 앞에 붙여야 합니다.\n");
            printf("다시 입력해 주세요 : ");
            scanf("%d %d", &aimX, &aimY);
        }

        if(aimX < 0 && aimY < 0) {
            aimX *= -1; aimY *= -1;
            aimX--; aimY--;
            
            if(board[aimY][aimX].is_flag) {
                board[aimY][aimX].is_flag = 0;
                flaged--;
            }
            else if(board[aimY][aimX].is_open == 0) {
                board[aimY][aimX].is_flag = 1;
                flaged++;
                }
                else printf("이미 열린 칸에 깃발을 설치할 수 없습니다.\n");
            continue;
        }
        
        aimX--; aimY--;
        revealed++;
        reveal(aimX, aimY);

        if(game_over) {
            printf("\n(%d,%d)은 지뢰였습니다. 게임 오버!\n", aimX + 1, aimY + 1);
            printf("소요 시간 : %d초", (int)(time(NULL) - start_time));
           return 0;
        }
        if(n * m == c + revealed) {
            printf("\n지뢰를 모두 찾았습니다. 게임 승리!\n");
            printf("소요 시간 : %d초", (int)(time(NULL) - start_time));
            return 0;
        }
    }
}

void init() {
    while(1) {
        printf("가로 크기를 입력해 주세요 (4~20) : ");
        scanf("%d", &n);
        if(4 <= n && n <= 20) break;
        else printf("다시 입력해 주세요. ");
    }

    while(1) {
        printf("세로 크기를 입력해 주세요 (4~20) : ");
        scanf("%d", &m);
        if(4 <= m && m <= 20) break;
        else printf("다시 입력해 주세요. ");
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
            board[j][i].is_flag = 0;
            board[j][i].is_mine = 0;
            board[j][i].is_open = 0;
        }
    }
}

void print_board(int x, int y) {
    printf("\n----------------------------------------------");
    printf("\n지뢰 수 : %d, 전체 칸 : %d, 남은 지뢰 : %d %d\n  ", c, n * m, c - flaged, revealed);
    for(int i = 0; i < n; i++)
        printf(" %2d", i + 1);
    printf("\n");

    for(int i = 0; i < m; i++) {
        printf("%2d)", i + 1);

        for(int j = 0; j < n; j++) {
        background_color(base_color);
            if(board[i][j].is_flag == 1) {
                font_color(flag_color);
                printf(" %c ", flag);
            }
            else if(board[i][j].is_open == 1) {
                if(board[i][j].adj) {
                    switch(board[i][j].adj) {
                        case 1 : font_color(color_1); break;
                        case 2 : font_color(color_2); break;
                        case 3 : font_color(color_3); break;
                        case 4 : font_color(color_4); break;
                        case 5 : font_color(color_5); break;
                    }

                printf(" %d ", board[i][j].adj);
                }
                else printf(" %c ", opened);
                }
                else {
                    printf(" %c ", closed);
                }
        font_color(0, 0, 0);
        }
        background_color(0, 0, 0);
        printf("(%d\n", i + 1);
    }
    printf("  ");
    for(int i = 0; i < n; i++)
        printf(" %2d", i + 1);
}

void set_mine(int x, int y) {
    int tempX, tempY;
    int count;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(count = 0; count < c; count++) {
        srand(clock() + count);                      // time(NULL) 의 경우 1초 단위로 업데이트되어 반복문에서 사용 부적합
        tempX = (rand() * rand() + count * x) % n;   // 최대한의 무작위성을 가질 수 있도록 변하는 값들을 활용
        tempY = (rand() * rand() + count * y) % m;

        if((abs(tempX - x) > 1 || abs(tempY - y) > 1) && board[tempY][tempX].is_mine == 0)
            board[tempY][tempX].is_mine = 1;         // 첫 칸 주위 1칸에는 지뢰를 배치하지 않도록 하여 초반 억까 방지
        else count--;
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            count = 0;
            for(int k = 0; k < 8; k++) {
                if(0 <= (i + dx[k]) && (i + dx[k]) < n) {
                    if(0 <= (j + dy[k]) && (j + dy[k]) < n) {
                        if(board[j + dy[k]][i + dx[k]].is_mine == 1)
                            count++;
                    }
                }
            }
            board[j][i].adj = count;
        }
    }
}

void reveal(int x, int y) {
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    board[y][x].is_open = 1;
    if(board[y][x].is_mine == 1){   // 사용자가 직접 지뢰 칸을 열었을 때만 실행됨.
        game_over = 1;
        return;
    }                               // 재귀에서 지뢰 칸은 그 전에 해당 구문(if~adj)에서 return당하여 실행되지 않음
    if(board[y][x].adj) return;     // 지뢰 칸에 도달하기 전에 adj가 양수인 칸을 반드시 지나야 하기 때문

    for(int k = 0; k < 8; k++) {
        if(0 <= (x + dx[k]) && (x + dx[k]) < n) {
            if(0 <= (y + dy[k]) && (y + dy[k]) < m) {
                if(board[y + dy[k]][x + dx[k]].is_open == 0) {
                    board[y + dy[k]][x + dx[k]].is_open = 1;
                    revealed++;
                    reveal(x + dx[k], y + dy[k]);
                }
            }
        }
    }
}