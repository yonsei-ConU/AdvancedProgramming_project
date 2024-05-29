#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "color.h"

#define mine '#'
#define closed '@'
#define opened '_'
#define flag '!'

#define bool int
#define true 1
#define false 0

int n, m, c = 0;        // n은 가로, m은 세로, c는 지뢰 개수
int revealed = 0;       // 1개의 칸 열릴때마다 1 증가
int flaged = 0;         // 깃발 1개 꽂을때마다 1 증가
bool game_over = false; // 지뢰 칸 열면 1로 변경
int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

typedef struct {
    bool is_mine; // 기본값 0, 지뢰일 경우 1
    bool is_flag; // 기본값 0, 깃발 세워졌을 경우 1
    bool is_open; // 기본값 0, 열렸을 경우 1
    int adj;      // 인접한 칸의 지뢰 개수
} Board;

Board board[20][20]; // board[y좌표][x좌표]

void init();
void print_board(int x, int y);
void set_mine(int x, int y);
void reveal(int x, int y);

int main() {
    int aimX, aimY; // 사용자 관점에서의 좌표(1부터 시작)으로 입력은 받으나 1씩 감소시켜 0 시작(배열 관점)으로 변형

    init();
    print_board(-1, -1);

    printf("\nx, y 좌표를 공백으로 구분하여 입력해 주세요.\n");
    printf("처음 여는 칸과 그 주변은 지뢰가 아닙니다.\n");
    printf("범위 x(1~%d) y(1~%d) : ", n, m);
    scanf("%d %d", &aimX, &aimY);

    if (c > (n * m - 9) / 5 * 4) {
        printf("\n*지뢰의 개수가 많아 시간이 오래 걸릴 수 있습니다. 기다려 주세요\n");
    }
    
    aimX--; aimY--; // 배열 관점의 좌표로 변경

    set_mine(aimX, aimY);
    reveal(aimX, aimY);

    if (n * m == c + revealed) {
        print_board(-1, -1);
        printf("\n시작하자마자 게임이 끝났군요!\n%d개의 지뢰를 모두 찾았습니다. 게임 승리.\n", c);
        return 0;
    }

    time_t start_time = time(NULL); // 소요 시간 측정용도로 시작 시간 저장

    while (1) {
        print_board(aimX, aimY);
        
        printf("\n(%d,%d)를 입력하였습니다. 새 좌표를 입력해 주세요 : ", aimX + 1, aimY + 1);
        scanf("%d %d", &aimX, &aimY);

        while (!(1 <= abs(aimX) && abs(aimX) <= n) || !(1 <= abs(aimY) && abs(aimY) <= m)) {
            printf("잘못된 입력입니다. 범위에 맞춰 다시 입력해 주세요.\n");
            printf("다시 입력해 주세요 : ");
            scanf("%d %d", &aimX, &aimY);
        }

        if (aimX < 0 || aimY < 0) {
            aimX = abs(aimX); aimY = abs(aimY);
            aimX--; aimY--;
            
            if (board[aimY][aimX].is_flag) {
                board[aimY][aimX].is_flag = false;
                flaged--;
            } else if (!board[aimY][aimX].is_open) {
                board[aimY][aimX].is_flag = true;
                flaged++;
            } else {
                printf("이미 열린 칸에 깃발을 설치할 수 없습니다.\n");
            }
            continue;
        }
        
        aimX--; aimY--;
        reveal(aimX, aimY);

        if (game_over) {
            print_board(-1, -1);
            printf("\n(%d,%d)은 지뢰였습니다. 게임 오버!\n", aimX + 1, aimY + 1);
            printf("소요 시간 : %d초", (int)(time(NULL) - start_time));
            return 0;
        }
        if (n * m == c + revealed) {
            print_board(-1, -1);
            printf("\n%d개의 지뢰를 모두 찾았습니다. 게임 승리!\n", c);
            printf("소요 시간 : %d초", (int)(time(NULL) - start_time));
            return 0;
        }
    }
}

void init() {
    printf("##지뢰 찾기##\n\n");
    printf("지뢰 찾기는 직사각형의 판 안에서 모든 지뢰를 찾아내면 승리하는 간단한 게임입니다.\n");
    printf("기본적인 조작은 좌표 입력으로 진행되며 왼쪽 위에서 1부터 시작합니다.\n\n");
    printf("좌표는 공백으로 구분됩니다. 칸에 적힌 숫자는 인접한 칸에 있는 지뢰의 수를 의미합니다.\n");
    printf("좌표 앞에 -를 한 번 이상 붙일 경우 (ex. -4 7 또는 -9 -2) 깃발을 설치하는 것으로 간주됩니다.\n");
    printf("깃발이 설치된 자리에 깃발을 다시 설치하면 깃발이 없어집니다.\n");
    printf("직전에 선택한 칸은 빨간색 배경으로 표시됩니다.\n\n");
    printf("게임을 시작합니다.\n");

    while (1) {
        printf("가로 크기를 입력해 주세요 (5~20) : ");
        scanf("%d", &n);
        if (5 <= n && n <= 20) break;
        else printf("다시 입력해 주세요, ");
    }

    while (1) {
        printf("세로 크기를 입력해 주세요 (5~20) : ");
        scanf("%d", &m);
        if (5 <= m && m <= 20) break;
        else printf("다시 입력해 주세요, ");
    }

    while (1) {
        printf("\n추천 난이도 : 쉬움(%d) 보통(%d) 어려움(%d)\n",
            ((n * m - 9) / 10), ((n * m - 9) / 6), ((n * m - 9) / 4));
        printf("지뢰의 개수를 입력해 주세요 (1~%d) : ", n * m - 9);
        scanf("%d", &c);
        if (1 <= c && c <= n * m - 9) break;
        else printf("다시 입력해 주세요. ");
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j].is_flag = false;
            board[i][j].is_mine = false;
            board[i][j].is_open = false;
            board[i][j].adj = 0;
        }
    }
}

void print_board(int x, int y) {
    printf("\n지뢰 수 : %d, 전체 칸 : %d, 남은 지뢰 : %d\n", c, n * m, c - flaged);
    printf("  ");
    for (int i = 0; i < n; i++) {
        printf(" %2d", i + 1);
    }
    printf("\n");

    for (int i = 0; i < m; i++) {
        printf("%2d)", i + 1);

        for (int j = 0; j < n; j++) {
            if (game_over && board[i][j].is_mine) {
                if (!board[i][j].is_flag) {
                    printf(" %c ", mine);
                }
            } else {
                if (board[i][j].is_flag) {
                    printf(" %c ", flag);
                } else if (board[i][j].is_open) {
                    if (board[i][j].adj) {
                        printf(" %d ", board[i][j].adj);
                    } else {
                        printf(" %c ", opened);
                    }
                } else {
                    printf(" %c ", closed);
                }
            }
        }
        printf("(%d\n", i + 1);
    }

    printf("  ");
    for (int i = 0; i < n; i++) {
        printf(" %2d", i + 1);
    }
    printf("\n");
}
void set_mine(int x, int y) {
    int tempX, tempY;
    int count;

    for (count = 0; count < c; count++) {
        srand(clock() + count); // time(NULL) 대신 clock()을 사용하여 무작위성을 높임
        tempX = rand() % n;
        tempY = rand() % m;

        // 첫 클릭한 칸과 그 주변 1칸에는 지뢰를 배치하지 않도록 함
        if ((abs(tempX - x) > 1 || abs(tempY - y) > 1) && !board[tempY][tempX].is_mine) {
            board[tempY][tempX].is_mine = true;
        } else {
            count--; // 조건에 맞지 않으면 카운트를 증가시키지 않음
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int mine_count = 0;
            if (!board[i][j].is_mine) {
                for (int k = 0; k < 8; k++) {
                    int ni = i + dy[k];
                    int nj = j + dx[k];
                    if (ni >= 0 && ni < m && nj >= 0 && nj < n && board[ni][nj].is_mine) {
                        mine_count++;
                    }
                }
                board[i][j].adj = mine_count;
            }
        }
    }
}

void reveal(int x, int y) {
    if (x < 0 || x >= n || y < 0 || y >= m) return; // 범위 벗어나면 리턴
    if (board[y][x].is_open || board[y][x].is_flag) return; // 이미 열렸거나 깃발이 있으면 리턴

    board[y][x].is_open = true;
    revealed++;

    if (board[y][x].is_mine) {
        game_over = true;
        return;
    }

    if (board[y][x].adj == 0) {
        for (int k = 0; k < 8; k++) {
            int ni = y + dy[k];
            int nj = x + dx[k];
            reveal(nj, ni); // 인접한 칸을 재귀적으로 엶
        }
    }
}