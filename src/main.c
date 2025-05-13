#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define LARGURA_JOGO 24
#define ALTURA_JOGO 18
#define INICIO_X 8
#define INICIO_Y 4

int posicao_x, posicao_y;
int rotacao, id_peca, proxima_peca;
int pontuacao = 0;
int velocidade = 300000;

char *tetraminos[7];
char area_jogo[ALTURA_JOGO][LARGURA_JOGO];

void gerar_tetraminos() {
    tetraminos[0] = "...."
                    "XXXX"
                    "...."
                    "...."; // I

    tetraminos[1] = "..X."
                    "..X."
                    ".XX."
                    "...."; // J

    tetraminos[2] = ".X.."
                    ".X.."
                    ".XX."
                    "...."; // L

    tetraminos[3] = "..X."
                    ".XX."
                    ".X.."
                    "...."; // T

    tetraminos[4] = "...."
                    ".XX."
                    ".XX."
                    "...."; // O

    tetraminos[5] = "..X."
                    ".XX."
                    ".X.."
                    "...."; // S

    tetraminos[6] = ".X.."
                    ".XX."
                    "..X."
                    "...."; // Z
}

int rotacionar_peca(int pX, int pY, int r) {
    switch (r % 4) {
        case 0: return pY * 4 + pX;             // 0°
        case 1: return 12 + pY - (pX * 4);      // 90°
        case 2: return 15 - (pY * 4) - pX;      // 180°
        case 3: return 3 - pY + (pX * 4);       // 270°
    }
    return 0;
}

int checar_colisao(char *peca, int px, int py, int r) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int i = rotacionar_peca(x, y, r);
            if (peca[i] == 'X') {
                int novo_x = px + x;
                int novo_y = py + y;

                if (novo_x < 0 || novo_x >= LARGURA_JOGO || novo_y >= ALTURA_JOGO)
                    return 1;
                if (novo_y >= 0 && area_jogo[novo_y][novo_x] != ' ')
                    return 1;
            }
        }
    }
    return 0;
}

void desenhar_peca(char *peca, int px, int py, int r, char simbolo) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int i = rotacionar_peca(x, y, r);
            if (peca[i] == 'X') {
                int desenhar_x = px + x;
                int desenhar_y = py + y;

                if (desenhar_x >= 0 && desenhar_x < LARGURA_JOGO &&
                    desenhar_y >= 0 && desenhar_y < ALTURA_JOGO) {
                    screenGotoxy(INICIO_X + desenhar_x, INICIO_Y + desenhar_y);
                    printf("%c", simbolo);
                }
            }
        }
    }
}

void fixar_peca(char *peca, int px, int py, int r) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int i = rotacionar_peca(x, y, r);
            if (peca[i] == 'X') {
                int fx = px + x;
                int fy = py + y;
                if (fx >= 0 && fx < LARGURA_JOGO && fy >= 0 && fy < ALTURA_JOGO)
                    area_jogo[fy][fx] = 'X';
            }
        }
    }
}


void limpar_linhas() {
    for (int y = 0; y < ALTURA_JOGO; y++) {
        int linha_completa = 1;
        for (int x = 0; x < LARGURA_JOGO; x++) {
            if (area_jogo[y][x] == ' ') {
                linha_completa = 0;
                break;
            }
        }
        if (linha_completa) {
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < LARGURA_JOGO; x++) {
                    area_jogo[ny][x] = area_jogo[ny - 1][x];
                }
            }
            for (int x = 0; x < LARGURA_JOGO; x++) {
                area_jogo[0][x] = ' ';
            }
            pontuacao += 100;
            if (velocidade > 100000)
                velocidade -= 10000;
            timerUpdateTimer(velocidade);
        }
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

void exibir_pontuacao() {
    screenGotoxy(INICIO_X + LARGURA_JOGO + 2, INICIO_Y);
    printf("Pontuacao: %d", pontuacao);
}

void exibir_proxima_peca() {
    screenGotoxy(INICIO_X + LARGURA_JOGO + 2, INICIO_Y + 2);
    printf("Proxima:");
    for (int y = 0; y < 4; y++) {
        screenGotoxy(INICIO_X + LARGURA_JOGO + 2, INICIO_Y + 3 + y);
        for (int x = 0; x < 4; x++) {
            char bloco = tetraminos[proxima_peca][rotacionar_peca(x, y, 0)];
            printf("%c", bloco == 'X' ? 'X' : ' ');
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

void nova_peca() {
    id_peca = proxima_peca;
    proxima_peca = rand() % 7;
    posicao_x = LARGURA_JOGO / 2 - 2;
    posicao_y = 0;
    rotacao = 0;
}

int main() {
    srand(time(NULL));
    screenInit(1);
    keyboardInit();
    timerInit(velocidade);

    gerar_tetraminos();
    inicializar_area_jogo();
    desenhar_bordas();

    proxima_peca = rand() % 7;
    nova_peca();

    while (1) {
        if (keyhit()) {
            int tecla = readch();
            if (tecla == 'a') {
                if (!checar_colisao(tetraminos[id_peca], posicao_x - 1, posicao_y, rotacao))
                    posicao_x--;
            } else if (tecla == 'd') {
                if (!checar_colisao(tetraminos[id_peca], posicao_x + 1, posicao_y, rotacao))
                    posicao_x++;
            } else if (tecla == 's') {
                if (!checar_colisao(tetraminos[id_peca], posicao_x, posicao_y + 1, rotacao))
                    posicao_y++;
            } else if (tecla == 'w') {
                if (!checar_colisao(tetraminos[id_peca], posicao_x, posicao_y, rotacao + 1))
                    rotacao++;
            }
        }

        if (timerTimeOver()) {
            if (!checar_colisao(tetraminos[id_peca], posicao_x, posicao_y + 1, rotacao)) {
                posicao_y++;
            } else {
                fixar_peca(tetraminos[id_peca], posicao_x, posicao_y, rotacao);
                limpar_linhas();
                nova_peca();
                if (checar_colisao(tetraminos[id_peca], posicao_x, posicao_y, rotacao)) {
                    screenGotoxy(INICIO_X, INICIO_Y + ALTURA_JOGO + 2);
                    printf("Game Over! Pontuacao final: %d\n", pontuacao);
                    break;
                }
            }
            timerUpdateTimer(velocidade);
        }

        desenhar_area_jogo();
        desenhar_peca(tetraminos[id_peca], posicao_x, posicao_y, rotacao, 'X');
        exibir_pontuacao();
        exibir_proxima_peca();

        screenUpdate();
        usleep(5000);
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    return 0;
}

 
