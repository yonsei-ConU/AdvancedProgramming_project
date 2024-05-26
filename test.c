#include <stdio.h>
#include <string.h>

char* color(int font, int background);

int main() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++)
            printf("%shello(%d/%d)  ", color(i, j), i, j);
        printf("\n");
    }
    printf("%s", color(0, 0));

    for(int i = 0; i < 8; i++) {
        printf("%shello(%d/%d)  ", color(i, 8), i, 8);
        printf("%shello(%d/%d)\n", color(i, 0), i, 0);
    }
    printf("%sm%smhello %sm%smhello\n", "\033[38;2;0;255;0", "\033[48;2;0;0;0", "\033[38;2;0;255;0", "\033[48;2;70;70;70");
    printf("%sm%smhello %sm%smhello\n", "\033[38;2;255;255;255", "\033[48;2;0;0;0", "\033[38;2;255;255;255", "\033[48;2;70;70;70");
    printf("hello\n");
    printf("%s", color(0, 0));
    return 0;
}

// https://velog.io/@chez_bono/C-%EC%BD%98%EC%86%94%EC%97%90-%EC%B6%9C%EB%A0%A5%EB%90%98%EB%8A%94-%ED%85%8D%EC%8A%A4%ED%8A%B8-%EC%83%89%EC%83%81-%EB%B3%80%EA%B2%BD

char* color(int font, int background) {
    static char s1[30];
    char s2[30];

    if(font == 0 && background == 0) return "\033[0m";

    sprintf(s1, "\033[38;5;%dm", font);
    sprintf(s2, "\033[48;5;%dm", background);
    strcat(s1, s2);

    return s1;
}