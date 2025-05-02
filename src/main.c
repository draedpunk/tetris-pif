#include <stdio.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <unistd.h>


int main() {
    int x = (MAXX - MINX) / 2;
    int y = 1;

    screenInit(1);         
    keyboardInit();        
    timerInit(300);       

    while (1) {
        if (keyhit()) {
            char key = readch();
            if (key == 'q') break;
            if (key == 'a' && x > MINX + 1) x--;  //esquerda
            if (key == 'd' && x < MAXX - 1) x++;  //direita
            if (key == 's' && y < MAXY - 1) y++;  //pra baixo
        }

        if (timerTimeOver()) {
            y++;
            if (y >= MAXY - 1) y = 1;
        }

        screenClear();
        screenDrawBorders();
        screenGotoxy(x, y);
        printf("⬛");
        usleep(20000);
        fflush(stdout);
    }

    screenDestroy();
    keyboardDestroy();
    return 0;
}
