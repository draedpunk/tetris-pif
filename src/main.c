#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tetris.h"
#include "ui.h"

#define LARGURA_JOGO 9
#define ALTURA_JOGO 18
#define INICIO_X 6
#define INICIO_Y 4

char *tetraminos[9];

void alocar_mapa(MAPA* t) {
    t->matriz = malloc(t->linhas * sizeof(char*));

    for (int i = 0; i < t->linhas; i++) {
        t->matriz[i] = malloc((t->colunas + 1) * sizeof(char));
        for (int j = 0; j < t->colunas + 1; j++) {
            t->matriz[i][j] = 0;
        }
    }
}


void ler_mapa(MAPA *t) {
    FILE *f = fopen("mapa.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o mapa.\n");
        exit(1);
    }

    char linha[TAMANHO_MAX_COLUNAS + 2];
    int linhas = 0;
    int colunas = -1;

    char temp[TAMANHO_MAX_LINHAS][TAMANHO_MAX_COLUNAS];

    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = 0; 
        int len = strlen(linha);

        if (colunas == -1) colunas = len;
        if (len != colunas) {
            printf("Erro: mapa com colunas irregulares (linha %d).\n", linhas + 1);
            fclose(f);
            exit(1);
        }

        if (linhas >= TAMANHO_MAX_LINHAS) {
            printf("Erro: mapa excede o número máximo de linhas.\n");
            fclose(f);
            exit(1);
        }

        strcpy(temp[linhas], linha);
        linhas++;
    }

    fclose(f);

    t->linhas = linhas;
    t->colunas = colunas;
    alocar_mapa(t); 

    for (int i = 0; i < linhas; i++) {
        strcpy(t->matriz[i], temp[i]);
    }
    printf("Debug - First line of map: %s\n", t->matriz[0]);

}

void imprimir_mapa(MAPA* t) {
    for (int i = 0; i < t->linhas; i++) {
        screenGotoxy(INICIO_X, INICIO_Y + i);
        for (int j = 0; j < t->colunas; j++) {
            printf("%c", t->matriz[i][j]);
        }
    }
}

//peças do tetris: I, O, T, S, L
// peças espelhadas: S-> Z, L -> J
// peça explosiva 
void carregar_tetraminos() {
    tetraminos[0] = "...."
                    "XXXX"
                    "...."
                    "...."; // I

    tetraminos[1] = "..X."
                    "..X."
                    ".XX."
                    "...."; // J pra simular o L espelhado

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


void ler_input(int tecla[4]) {
    for (int i = 0; i < 4; i++) tecla[i] = 0;

    if (keyhit()) {
        char t = readch();
        switch (t) {
            case 'd': case 'D': tecla[0] = 1; break; // direita
            case 'a': case 'A': tecla[1] = 1; break; // esquerda
            case 's': case 'S': tecla[2] = 1; break; // baixo
            case 'w': case 'W': tecla[3] = 1; break; // rotacionar
        }
    }
}

void explodir(MAPA* t, int cx, int cy) {
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int px = cx + x;
            int py = cy + y;
            if (px >= 0 && px < LARGURA_JOGO && py >= 0 && py < ALTURA_JOGO) {
                t->matriz[py][px] = ' ';
            }
        }
    }
}


void desenhar_mapa_com_peca(MAPA* t, int tetramino_atual, int rotacao_atual, int x_atual, int y_atual) {
    for (int y = 0; y < t->linhas; y++) {
        for (int x = 0; x < t->colunas; x++) {
            screenGotoxy(INICIO_X + x, INICIO_Y + y);

            int caractere_peca = 0;

            int rel_x = x - x_atual;
            int rel_y = y - y_atual;
            if (rel_x >= 0 && rel_x < 4 && rel_y >= 0 && rel_y < 4) {
                int pi = rotacionar(rel_x, rel_y, rotacao_atual);
                if (tetraminos[tetramino_atual][pi] != '.') {
                    caractere_peca = 1;
                }
            }

            if (caractere_peca) {
                printf("%c", 'A' + tetramino_atual);
            } else {
                printf("%c", t->matriz[y][x]);
            }
        }
        printf("\n");
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

void inicializar_mapa(MAPA *t) {
    for (int y = 0; y < t->linhas; y++) {
        for (int x = 0; x < t->colunas; x++) {
            t->matriz[y][x] = ' ';
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

void atualizar_pontuacao(int *pontuacao, int linhas, int eh_explosiva) {
    if (eh_explosiva) {
        *pontuacao -= 50; //vai perder 50 pontos com a peça explsoiva
        return;
    }
    
    *pontuacao += 25;
    if (linhas > 0) {
        *pontuacao += (1 << linhas) * 100;
    }
}

void processar_input(int *teclas, int *x, int *y, int *rot, int *bRotateHold, MAPA *t, int tipo) {
    if (teclas[0] && pode_encaixar(t, tipo, *rot, *x + 1, *y)) (*x)++;
    if (teclas[1] && pode_encaixar(t, tipo, *rot, *x - 1, *y)) (*x)--;
    if (teclas[2] && pode_encaixar(t, tipo, *rot, *x, *y + 1)) {
        (*y)++;
        timerUpdateTimer(500);
    }
    if (teclas[3]) {
        if (*bRotateHold && pode_encaixar(t, tipo, *rot + 1, *x, *y))
            (*rot)++;
        *bRotateHold = 0;
    } else {
        *bRotateHold = 1;
    }
}

int verificar_game_over(MAPA *t, int tipo, int rot, int x, int y) {
    return !pode_encaixar(t, tipo, rot, x, y);
}

int main() {
    MAPA t;
    ler_mapa(&t); 
    srand(time(NULL));
    carregar_tetraminos();
    exibir_banner_titulo();
    screenInit(1);
    
    timerInit(500);

    int pontuacao = 0, fim_jogo = 0, velocidade = 500;
    int teclas[4] = {0}, bRotateHold = 1;

    int tipo = rand() % 9, rot = 0;
    int x = LARGURA_JOGO / 2 - 2, y = 0;

    while (!fim_jogo) {
        int cair = timerTimeOver();
        ler_input(teclas);
        processar_input(teclas, &x, &y, &rot, &bRotateHold, &t, tipo);

        if (cair && pode_encaixar(&t, tipo, rot, x, y + 1)) {
            y++;
            timerUpdateTimer(velocidade);
        } else if (!pode_encaixar(&t, tipo, rot, x, y + 1)) {
            posicionar_tetramino_no_mapa(&t, tipo, rot, x, y);

            if (tipo == 8) explodir(&t, x + 1, y + 1);

            int linhas = remover_linhas_completas(&t);
            atualizar_pontuacao(&pontuacao, linhas, (tipo == 8));

            tipo = rand() % 9;
            rot = 0;
            x = LARGURA_JOGO / 2 - 2;
            y = 0;

            if (verificar_game_over(&t, tipo, rot, x, y)) {
                fim_jogo = 1;
                screenGotoxy(INICIO_X, INICIO_Y + t.linhas / 2);
                //printf("GAME OVER!");
                exibir_banner_gameover();
            }
        }

        desenhar_mapa_com_peca(&t, tipo, rot, x, y);
        screenGotoxy(INICIO_X, INICIO_Y + t.linhas + 1);
        printf("Pontuacao: %d", pontuacao);
        screenUpdate();
        usleep(50000);
    }

    screenDestroy();
    for (int i = 0; i < t.linhas; i++) free(t.matriz[i]);
    free(t.matriz);
    return 0;
}