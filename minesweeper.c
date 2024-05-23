#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define mine '#'
#define closed '@'
#define opened '_'
#define flag '!'

int n, m, c; // n은 가로, m은 세로, c는 지뢰 개수
int revealed, flaged = 0;
int game_over = 0;

typedef struct{
    int is_mine;
    int is_flag;
    int is_open;
    int adj;            // 인접한 칸의 지뢰 개수
} Board;

Board board[20][20];    // n,m 값을 받아서 딱 맞게 선언할 수 있으나 간결함을 위해 미리 최댓값으로 설정

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
void print_board(); // x y 반전 주의
/**
 * @brief 지뢰의 위치를 정하고 각 자리의 adj(주변 지뢰의 개수)를 계산하는 함수.
 * @param x 시작지점의 x좌표 (0부터 시작)
 * @param y 시작지점의 y좌표 (0부터 시작)
 * @return 없음
 */
void set_mine(int x, int y); // 배열 관점에서의 좌표 (0부터 시작)
/**
 * @brief 지뢰의 칸을 여는 함수
 * @param x 열고자 하는 지점의 x좌표 (0부터 시작)
 * @param y 열고자 하는 지점의 y좌표 (0부터 시작)
 */
void reveal(int x, int y); // 배열 관점에서의 좌표 (0부터 시작)

int main() {
    int aimX, aimY; // 사용자 관점에서의 좌표 (1부터 시작)

    init();
    print_board();

    printf("좌표를 입력해 주세요(x y) : ");
    scanf("%d %d", &aimX, &aimY);

    set_mine(aimX - 1, aimY - 1);
    reveal(aimX - 1, aimY - 1);

    while(game_over != 0) {
        print_board();
        printf("좌표를 입력해 주세요(x y) : ");
        scanf("%d %d", &aimX, &aimY);
        reveal(aimX - 1, aimY - 1);
    }
    printf("지뢰 칸을 열었습니다! 게임 종료\n");
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
        printf("지뢰의 개수를 입력해 주세요 (1~%d) :", n * m - 9); // 초반 억까 방지용으로 자신과 주변 1칸인 9칸에는
        scanf("%d", &c);                                        // 지뢰가 들어갈 수 없기 때문에 9를 뺌
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
    printf("지뢰 수 : %d, 전체 칸 : %d, 남은 지뢰 : %d\n", c, n * m, c - flaged);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(board[j][i].is_flag == 1) {
                printf("%c ", flag);
            }
            else if(board[j][i].is_open == 1) {
                if(board[j][i].adj)
                    printf("%d ", board[j][i].adj);
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
        srand(clock() * count);             // time(NULL) 의 경우 1초 단위로 업데이트되어 반복문에서 사용 부적합으로 판단
        tempX = (rand() + x * count) % n;   // 최대한의 무작위성을 가질 수 있도록 변하는 값들을 활용
        tempY = (rand() + y * count) % m;

        if(abs(tempX - x) > 1 && abs(tempY - y) > 1)    // 첫 칸 주위 1칸에는 지뢰를 배치하지 않도록 하여 초반 억까 방지
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
    if(board[x][y].is_mine == 1){   // 사용자가 직접 지뢰 칸을 열었을 때만 실행됨.
        game_over = 1;
        return;
    }                               // 재귀에서 지뢰 칸은 그 전에 해당 구문(if~adj)에서 return당하여 실행되지 않음
    if(board[x][y].adj) return;     // 지뢰 칸에 도달하기 전에 adj가 양수인 칸을 반드시 지나야 하기 때문

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