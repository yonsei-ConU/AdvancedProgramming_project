#include <stdio.h>
#include <stdlib.h>

// \u2460 1원   \u25ef 큰 원    \u25cf 작은 검은색 원
// N은 가로, M은 세로

#define mine "#"
#define empty "_"

int n, m, c;

void init();

int main() {
    init();

    return 0;
}

void init() {
    while(1) {
        printf("가로 크기를 입력해 주세요 (3~20) : ");
        scanf("%d", &n);
        if(3 <= n && n <= 20) break;
        else printf("다시 입력해 주세요. ");
    }

    while(1) {
        printf("세로 크기를 입력해 주세요 (3~20) : ");
        scanf("%d", &m);
        if(3 <= m && m <= 20) break;
        else printf("다시 입력해 주세요. ");
    }

    while(1) {
        printf("지뢰의 개수를 입력해 주세요 (1~%d) :", n * m - 1);
        scanf("%d", &c);
        if(1 <= c && c <= n * m - 1) break;
        else printf("다시 입력해 주세요. ");
    }
}