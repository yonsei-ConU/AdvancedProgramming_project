#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// \u2460 1원   \u25ef 큰 원    \u25cf 작은 검은색 원

#define mine '#'
#define closed '@'
#define opened '_'
#define flag '!'

int n, m, c; // n은 가로, m은 세로, c는 지뢰 개수
int revealed, flaged = 0;

typedef struct{
    int is_mine;
    int is_flag;
    int is_open;
    int adj;
} Board;

Board board[20][20];

void init();
void print_board();
void set_mine(int x, int y); // 배열 관점에서의 좌표 (0부터 시작)
void reveal(int x, int y); // 배열 관점에서의 좌표 (0부터 시작)

int main() {
    int aimX, aimY; // 사용자 관점에서의 좌표 (1부터 시작)

    init();
    print_board();

    printf("좌표를 입력해 주세요(x y) : ");
    scanf("%d %d", &aimX, &aimY);

    set_mine(aimX - 1, aimY - 1);
    reveal(aimX - 1, aimY - 1);

    while(1) {
        print_board();
        printf("좌표를 입력해 주세요(x y) : ");
        scanf("%d %d", &aimX, &aimY);
        reveal(aimX - 1, aimY - 1);
    }
    return 0;
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
        printf("지뢰의 개수를 입력해 주세요 (1~%d) :", n * m - 9);
        scanf("%d", &c);
        if(1 <= c && c <= n * m - 9) break;
        else printf("다시 입력해 주세요. ");
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            board[i][j].is_flag = 0;
            board[i][j].is_mine = 0;
            board[i][j].is_open = 0;
        }
    }
}

void print_board() {
    printf("%d/%d\t남은 지뢰 : %d\n", c, n * m, c - flaged);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(board[i][j].is_flag == 1) {
                printf("%c ", flag);
            }
            else if(board[i][j].is_open == 1) {
                if(board[i][j].adj)
                    printf("%d ", board[i][j].adj);
                else printf("%c ", opened);
                }
                else printf("%c ", closed);
        }
        printf("\n");
    }
}

void set_mine(int x, int y) {
    int tempX, tempY;
    int count;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(count = 0; count < c; count++) {
        srand(clock() * count);
        tempX = (rand() + x * count) % n;
        tempY = (rand() + y * count) % m;

        if(abs(tempX - x) > 1 && abs(tempY - y) > 1)
            board[tempX][tempY].is_mine = 1;
        else count--;
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            count = 0;
            for(int k = 0; k < 8; k++) {
                if(0 <= (i + dx[k]) && (i + dx[k]) < n) {
                    if(0 <= (j + dy[k]) && (j + dy[k]) < n) {
                        if(board[i + dx[k]][j + dy[k]].is_mine == 1)
                            count++;
                    }
                }
            }
            board[i][j].adj = count;
        }
    }
}

void reveal(int x, int y) {
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    board[x][y].is_open = 1;
    if(board[x][y].is_mine == 1){
        printf("게임 종료\n");
        return;
    }
    if(board[x][y].adj) return;

    for(int k = 0; k < 8; k++) {
        if(0 <= (x + dx[k]) && (x + dx[k]) < n) {
            if(0 <= (y + dy[k]) && (y + dy[k]) < m) {
                if(board[x + dx[k]][y + dy[k]].is_open == 0) {
                    board[x + dx[k]][y + dy[k]].is_open = 1;
                    reveal(x + dx[k], y + dy[k]);
                }
            }
        }
    }
}