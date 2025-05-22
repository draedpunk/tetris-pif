#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "tetraminos.h"
#include "tetris.h"
#include "mapa.h"

char *tetraminos[9];

void carregar_tetraminos() {
    tetraminos[0] = "...."
                    "XXXX"
                    "...."
                    "...."; // I

    tetraminos[1] = "..X."
                    "..X."
                    ".XX."
                    "...."; // Jpra simular o L espelhado

    tetraminos[2] = ".X.."
                    ".X.."
                    ".XX."
                    "...."; // L

    tetraminos[3] = "..X."
                    ".XX."
                    "..X."
                    "...."; // T

    tetraminos[4] = ".X.."
                    ".XX."
                    ".X.."
                    "...."; // T espelhado

    tetraminos[5] = "...."
                    ".XX."
                    ".XX."
                    "...."; // O

    tetraminos[6] = "..X."
                    ".XX."
                    ".X.."
                    "...."; // S

    tetraminos[7] = ".X.."
                    ".XX."
                    "..X."
                    "...."; // Z
    
    tetraminos[8] = "000."
                    ".0.."
                    "...."
                    "...."; // peça explosiva
}

int rotacionar(int x, int y, int rot) {
    rot = rot % 4;
    switch(rot) {
        case 0: return y * 4 + x;           // 0 graus
        case 1: return 12 + y - (x * 4);    // 90 graus
        case 2: return 15 - (y * 4) - x;    // 180 graus
        case 3: return 3 - y + (x * 4);     // 270 graus
    }
    return 0; 
}

int pode_encaixar(MAPA* mapa, int tetramino, int rot, int posX, int posY) {
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            int pi = rotacionar(px, py, rot);
            int gx = posX + px;
            int gy = posY + py;

            if (tetraminos[tetramino][pi] != '.') {
                if (gx < 0 || gx >= mapa->colunas || gy < 0 || gy >= mapa->linhas) {
                    return 0;
                }

                if (mapa->matriz[gy][gx] != ' ') {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void explodir(MAPA* t, int cx, int cy) {
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int px = cx + x;
            int py = cy + y;
            if (px >= 0 && px < LARGURA_JOGO && py >= 0 && py < ALTURA_JOGO) {
                char alvo = t->matriz[py][px];
                if (alvo != '|' && alvo != '-') {
                    t->matriz[py][px] = ' ';
                }
            }
        }
    }
}

void posicionar_tetramino_no_mapa(MAPA *t, int tipo, int rot, int x, int y) {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            int pi = rotacionar(px, py, rot);
            if (tetraminos[tipo][pi] != '.') {
                int mx = x + px;
                int my = y + py;

                if (mx >= 0 && mx < t->colunas && my >= 0 && my < t->linhas) {
                    t->matriz[my][mx] = '#';
                }
            }
        }
    }
}

int remover_linhas_completas(MAPA *t) {
    int linhas_removidas = 0;

    for (int y = t->linhas - 2; y >= 0; y--) {
        int cheia = 1;
        for (int x = 1; x < t->colunas - 1; x++) {
            if (t->matriz[y][x] == ' ') {
                cheia = 0;
                break;
            }
        }

        if (cheia) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 1; x < t->colunas - 1; x++) {
                    t->matriz[yy][x] = t->matriz[yy - 1][x];
                }
            }
            for (int x = 1; x < t->colunas - 1; x++) {
                t->matriz[0][x] = ' ';
            }

            linhas_removidas++;
            y++;
        }
    }

    return linhas_removidas;
}

void exibir_prox_peca(int proxima_peca) {
    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 1);
    printf("+---Proxima--+");

    for (int y = 0; y < 4; y++) {
        screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 2 + y);
        printf("|    ");  // esquerda

        for (int x = 0; x < 4; x++) {
            char bloco = tetraminos[proxima_peca][rotacionar(x, y, 0)];
            printf("%c", bloco != '.' ? bloco : ' ');
        }

        printf("    |");  //direita
    }
    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 6);
    printf("+------------+");
}

void exibir_linhas_removidas(int total_linhas_remov) {
    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 9);
    printf("+---Linhas---+");
    for (int i = 0; i < 3; i++) {
        screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 10 + i);
        printf("|            |");
    }
    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 15);
    printf("+------------+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 18, INICIO_Y + 11);
    printf("%4d", total_linhas_remov);
}



