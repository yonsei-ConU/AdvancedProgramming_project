#include <stdio.h>

// \u2460 1원   \u25ef 큰 원    \u25cf 작은 검은색 원

#define mine "\u25ef"
#define empty '_'

int main() {
    printf("%s", mine);
    return 0;
}