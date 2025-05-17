#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define LARGURA_JOGO 10
#define ALTURA_JOGO 18
#define INICIO_X 8
#define INICIO_Y 4

char *tetraminos[7];
unsigned char *grade_jogo = NULL;

void carregar_tetraminos() {
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

int pode_encaixar(int tetramino, int rot, int posX, int posY) {
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            int pi = rotacionar(px, py, rot);
            int gx = posX + px;
            int gy = posY + py;

            if (gx >= 0 && gx < LARGURA_JOGO && gy >= 0 && gy < ALTURA_JOGO) {
                if (tetraminos[tetramino][pi] != '.' && grade_jogo[gy * LARGURA_JOGO + gx] != 0) {
                    return 0;
                }
            } else if (tetraminos[tetramino][pi] != '.') {
                return 0; 
            }
        }
    }
    return 1;
}

void desenhar_area_jogo() {
    for (int y = 0; y < ALTURA_JOGO; y++) {
        screenGotoxy(INICIO_X - 1, INICIO_Y + y);
        printf("|");  // borda esquerda

        for (int x = 0; x < LARGURA_JOGO; x++) {
            unsigned char bloco = grade_jogo[y * LARGURA_JOGO + x];
            if (bloco == 0) {
                printf("");
            } else if (bloco == 8) {
                printf("="); // linha preenchida
            } else if (bloco == 9) {
                printf("#"); // parede/borda fixa
            } else {
                printf("%c", 'A' + bloco - 1);// tipo da peça
            }
        }

        printf("|");  // borda direita
    }

    // borda inferior
    screenGotoxy(INICIO_X - 1, INICIO_Y + ALTURA_JOGO);
    for (int i = 0; i < LARGURA_JOGO + 2; i++) {
        printf("#");
    }
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

int main(){

    srand(time(NULL));
    grade_jogo = (unsigned char *) calloc(LARGURA_JOGO * ALTURA_JOGO, sizeof(unsigned char));

    if (!grade_jogo) {
        printf("Erro na alocação de memória!\n");
        return 1;
    }
    carregar_tetraminos();

    // vai começar a grade de jogo com paredes nas bordas e chao,e 0 pro resto
    for (int y = 0; y < ALTURA_JOGO; y++) {
        for (int x = 0; x < LARGURA_JOGO; x++) {
            if (x == 0 || x == LARGURA_JOGO - 1 || y == ALTURA_JOGO - 1) {
                grade_jogo[y * LARGURA_JOGO + x] = 9; // bordas
            } else {
                grade_jogo[y * LARGURA_JOGO + x] = 0;
            }
        }
    }

    screenInit(1);

    int tetramino_atual = rand() % 7;
    int rotacao_atual = 0;
    int x_atual = LARGURA_JOGO / 2 - 2;
    int y_atual = 0;

    int velocidade = 500;
    int cair = 0;
    int tetraminos_jogados = 0;
    int pontuacao = 0;
    int fim_jogo = 0;
    int vetor_linhas[4];
    int total_linhas = 0;
    int bRotateHold = 1;
    int teclas[4] = {0, 0, 0, 0};

    while (!fim_jogo) {
        //timerInit(velocidade);
        cair = timerTimeOver();

        ler_input(teclas);

        if (teclas[0] && pode_encaixar(tetramino_atual, rotacao_atual, x_atual + 1, y_atual)) x_atual++;
        if (teclas[1] && pode_encaixar(tetramino_atual, rotacao_atual, x_atual - 1, y_atual)) x_atual--;
        if (teclas[2] && pode_encaixar(tetramino_atual, rotacao_atual, x_atual, y_atual + 1)) y_atual++;
        if (teclas[3]) {
            if (bRotateHold && pode_encaixar(tetramino_atual, rotacao_atual + 1, x_atual, y_atual))
                rotacao_atual++;
            bRotateHold = 0;
        } else {
            bRotateHold = 1;
        }

        /* a partir daqui */

        int pode_descer = pode_encaixar(tetramino_atual, rotacao_atual, x_atual, y_atual + 1);

        // Se for tempo de cair ou se o jogador tentou mover, atualiza a posição
        if (cair && pode_descer) {
            y_atual++;
            cair = 0;
        } else if (!pode_descer) {
            // A peça não pode mais descer → fixa imediatamente
            tetraminos_jogados++;

            if (tetraminos_jogados % 50 == 0 && velocidade >= 100) {
                velocidade -= 50;
                timerUpdateTimer(velocidade);
            }

            // FIXAR A PEÇA NA GRADE
            for (int px = 0; px < 4; px++) {
                for (int py = 0; py < 4; py++) {
                    if (tetraminos[tetramino_atual][rotacionar(px, py, rotacao_atual)] != '.') {
                        grade_jogo[(y_atual + py) * LARGURA_JOGO + (x_atual + px)] = tetramino_atual + 1;
                    }
                }
            }




                // verificar linhas completas
            total_linhas = 0;
            for (int py = 0; py < 4; py++) {
                int linha = y_atual + py;
                if (linha < ALTURA_JOGO - 1) {
                    int cheia = 1;
                    for (int px = 1; px < LARGURA_JOGO - 1; px++) {
                        if (grade_jogo[linha * LARGURA_JOGO + px] == 0) {
                            cheia = 0;
                            break;
                        }
                    }
                    if (cheia) {
                        for (int px = 1; px < LARGURA_JOGO - 1; px++) {
                            grade_jogo[linha * LARGURA_JOGO + px] = 8; // marcar linha cheia
                        }
                        if (total_linhas < 4) {
                            vetor_linhas[total_linhas++] = linha;
                        }
                    }
                }
            }

            pontuacao += 25;
            if (total_linhas > 0) {
                    pontuacao += (1 << total_linhas) * 100;

                    for (int i = 0; i < total_linhas; i++) {
                        for (int y = vetor_linhas[i]; y > 0; y--) {
                            for (int x = 1; x < LARGURA_JOGO - 1; x++) {
                                grade_jogo[y * LARGURA_JOGO + x] = grade_jogo[(y - 1) * LARGURA_JOGO + x];
                            }
                        }
                    }
                    total_linhas = 0;
            }

                // nova peça
            tetramino_atual = rand() % 7;
            rotacao_atual = 0;
            x_atual = LARGURA_JOGO / 2 - 2;
            y_atual = 0;

            if (!pode_encaixar(tetramino_atual, rotacao_atual, x_atual, y_atual)) {
                    fim_jogo = 1;
                    screenGotoxy(INICIO_X, INICIO_Y + ALTURA_JOGO / 2);
                    printf("GAME OVER!");
                }
            }

           desenhar_area_jogo();
        // Desenhar a peça atual no topo do jogo
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            int pi = rotacionar(px, py, rotacao_atual);
            if (tetraminos[tetramino_atual][pi] != '.') {
                screenGotoxy(INICIO_X + x_atual + px, INICIO_Y + y_atual + py);
                printf("%c", 'A' + tetramino_atual);
            }
        }
    }

    screenGotoxy(0, ALTURA_JOGO + INICIO_Y + 2);
    printf("Pontuacao: %d", pontuacao);
    screenUpdate();

    usleep(50000);
    }

 

    screenDestroy();
    free(grade_jogo);

    return 0;
}