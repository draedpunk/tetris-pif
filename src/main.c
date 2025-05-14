#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

/* area do jogo (onde as pecinhas vao cair) */
#define LARGURA_JOGO 24
#define ALTURA_JOGO 18
#define INICIO_X 8
#define INICIO_Y 4

// char *tetraminos[7];
char area_jogo[ALTURA_JOGO][LARGURA_JOGO];

int rotacionar(int posicao_x, int posicao_y, int grau_rotacao){
    int posicao_indice =0;
    switch(grau_rotacao % 4){
        // mudar pro codigo ascii das letrass
        case 0:
            posicao_indice = posicao_y* 4+ posicao_x;
            break;
        case 1:
            posicao_indice = 12 + posicao_y - (posicao_x*4);
            break;
        case 2:
            posicao_indice = 15 - (posicao_y*4) - posicao_x;
            break;
        case 3: 
            posicao_indice = 3 - posicao_y + (posicao_x*4);
            break;

    }
    return posicao_indice;
}

void desenhar_bordas() {
    for (int y = 0; y <= ALTURA_JOGO; y++) {
        screenGotoxy(INICIO_X - 1, INICIO_Y + y);
        printf("|");
        screenGotoxy(INICIO_X + LARGURA_JOGO, INICIO_Y + y);
        printf("|");
    }
    for (int x = -1; x <= LARGURA_JOGO; x++) {
        screenGotoxy(INICIO_X + x, INICIO_Y + ALTURA_JOGO);
        printf("#");
    }
}

void desenhar_area_jogo() {
    for (int y = 0; y < ALTURA_JOGO; y++) {
        for (int x = 0; x < LARGURA_JOGO; x++) {
            screenGotoxy(INICIO_X + x, INICIO_Y + y);
            printf("%c", area_jogo[y][x]);
        }
    }
}

void inicializar_area_jogo() {
    for (int y = 0; y < ALTURA_JOGO; y++) {
        for (int x = 0; x < LARGURA_JOGO; x++) {
            area_jogo[y][x] = ' ';
        }
    }
}

int main(){

    screenInit(1);
    inicializar_area_jogo();
    desenhar_bordas();

    screenDestroy();
    keyboardDestroy();

    return 0;
}