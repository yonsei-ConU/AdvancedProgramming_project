#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINE '#'
#define EMPTY '_'

int n, m, c;
char **board;
char **display;

void init();
void create_board();
void place_mines();
void calculate_numbers();
void print_board(char **board);
void play_game();
int count_adjacent_mines(int x, int y);
void reveal(int x, int y);
int is_valid(int x, int y);
void free_memory();

int main() {
    init();
    create_board();
    place_mines();
    calculate_numbers();
    play_game();
    free_memory();

    return 0;
}

void init() {
    while (1) {
        printf("가로 크기를 입력해 주세요 (3~20) : ");
        scanf("%d", &n);
        if (3 <= n && n <= 20) break;
        else printf("다시 입력해 주세요.\n");
    }

    while (1) {
        printf("세로 크기를 입력해 주세요 (3~20) : ");
        scanf("%d", &m);
        if (3 <= m && m <= 20) break;
        else printf("다시 입력해 주세요.\n");
    }

    while (1) {
        printf("지뢰의 개수를 입력해 주세요 (1~%d) : ", n * m - 1);
        scanf("%d", &c);
        if (1 <= c && c <= n * m - 1) break;
        else printf("다시 입력해 주세요.\n");
    }
}

void create_board() {
    board = (char **)malloc(n * sizeof(char *));
    display = (char **)malloc(n * sizeof(char *));
    if (board == NULL || display == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        board[i] = (char *)malloc(m * sizeof(char));
        display[i] = (char *)malloc(m * sizeof(char));
        if (board[i] == NULL || display[i] == NULL) {
            printf("메모리 할당 실패\n");
            exit(1);
        }

        for (int j = 0; j < m; j++) {
            board[i][j] = EMPTY;
            display[i][j] = EMPTY;
        }
    }
}

void place_mines() {
    srand(time(0));
    int mines_placed = 0;
    while (mines_placed < c) {
        int x = rand() % n;
        int y = rand() % m;
        if (board[x][y] != MINE) {
            board[x][y] = MINE;
            mines_placed++;
        }
    }
}

void calculate_numbers() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (board[i][j] == MINE) continue;
            int count = count_adjacent_mines(i, j);
            if (count > 0) {
                board[i][j] = '0' + count;
            }
        }
    }
}

int count_adjacent_mines(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            if (is_valid(x + i, y + j) && board[x + i][y + j] == MINE) {
                count++;
            }
        }
    }
    return count;
}

int is_valid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

void print_board(char **board) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void play_game() {
    int x, y;
    while (1) {
        print_board(display);
        printf("좌표를 입력해 주세요 (형식: x y): ");
        scanf("%d %d", &x, &y);
        if (x < 0 || x >= n || y < 0 || y >= m) {
            printf("잘못된 입력입니다. 다시 입력해 주세요.\n");
            continue;
        }
        if (board[x][y] == MINE) {
            printf("지뢰를 밟았습니다. 게임 오버!\n");
            print_board(board);
            break;
        }
        reveal(x, y);
        
        // Check if the game is won
        int cells_revealed = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (display[i][j] != EMPTY) {
                    cells_revealed++;
                }
            }
        }
        if (cells_revealed == n * m - c) {
            printf("축하합니다! 게임을 승리하였습니다.\n");
            print_board(board);
            break;
        }
    }
}

void reveal(int x, int y) {
    if (!is_valid(x, y) || display[x][y] != EMPTY) return;
    display[x][y] = board[x][y];
    if (board[x][y] == EMPTY) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;
                reveal(x + i, y + j);
            }
        }
    }
}

void free_memory() {
    for (int i = 0; i < n; i++) {
        free(board[i]);
        free(display[i]);
    }
    free(board);
    free(display);
}
