#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

/* area do jogo (onde as pecinhas vao cair) */
#define LARGURA_JOGO 12
#define ALTURA_JOGO 18
#define INICIO_X 8
#define INICIO_Y 4

char *tetraminos[7];
unsigned char *grade_jogo = NULL;
char area_jogo[ALTURA_JOGO][LARGURA_JOGO];

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

int pode_encaixar(int num_tetramino, int num_rotacao, int posX, int posY){
    for (int px =0; px <4; px++){
        for (int py = 0;py<4; py++){
            int index_tetramino = rotacionar(px, py, num_rotacao);
            int index_area_ocupada = (posY + py) * LARGURA_JOGO + (posX + px);

            if (posX + px >= 0 && posX + px < LARGURA_JOGO){
                if (posY + py >=0 && posY + py< ALTURA_JOGO){
                    if (tetraminos[num_tetramino][index_tetramino] != '.' && grade_jogo[index_area_ocupada] !=0){
                        return 0; // false
                    }
                }
            }
        } 
    }
    return 1; // true
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

void ler_input(int tecla[4])
{
    for (int i = 0; i < 4; i++) tecla[i] = 0;

    if (keyhit())
    {
        char tecla_pressionada = readch();

        switch (tecla_pressionada)
        {
            case 'd': case 'D': tecla[0] = 1; break;
            case 'a': case 'A': tecla[1] = 1; break;
            case 's': case 'S': tecla[2] = 1; break;
            case 'w': case 'W': tecla[3] = 1; break;
            // case 'p': case 'P': pausar = 1; break;   // Pausar
            // case 'r': case 'R': reiniciar = 1; break; // Reiniciar
            // case 27:            sair = 1; break;    // ESC (sair)
        }
    }
}

int main() {
    srand(time(NULL));
    int teclas[4] = {0, 0, 0, 0};

    grade_jogo = (unsigned char*)malloc(LARGURA_JOGO * ALTURA_JOGO * sizeof(unsigned char));
    if (grade_jogo == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }


    screenInit(1);
    inicializar_area_jogo();
    desenhar_bordas();
    carregar_tetraminos(); 

    for (int x = 0; x < LARGURA_JOGO; x++) {
        for (int y = 0; y < ALTURA_JOGO; y++) {
            grade_jogo[y * LARGURA_JOGO + x] = (x == 0 || x == LARGURA_JOGO - 1 || y == ALTURA_JOGO - 1) ? 9 : 0;
        }
    }

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

    while (!fim_jogo) {
        timerInit(velocidade);
        cair = timerTimeOver();

        ler_input(teclas);

        if (teclas[0] && pode_encaixar(tetramino_atual, rotacao_atual, x_atual + 1, y_atual))
            x_atual++;

        if (teclas[1] && pode_encaixar(tetramino_atual, rotacao_atual, x_atual - 1, y_atual))
            x_atual--;

        if (teclas[2] && pode_encaixar(tetramino_atual, rotacao_atual, x_atual, y_atual + 1))
            y_atual++;

        if (teclas[3]) {
            if (bRotateHold && pode_encaixar(tetramino_atual, rotacao_atual + 1, x_atual, y_atual))
                rotacao_atual++;
            bRotateHold = 0;
        } else {
            bRotateHold = 1;
        }

        if (cair) {
            cair = 0;
            tetraminos_jogados++;

            if (tetraminos_jogados % 50 == 0 && velocidade >= 100) {
                velocidade -= 50;
                timerUpdateTimer(velocidade);
            }

            if (pode_encaixar(tetramino_atual, rotacao_atual, x_atual, y_atual + 1)) {
                y_atual++;
            } else {
                // fixar a caceta da peça!!!!!!!!!!
                for (int px = 0; px < 4; px++) {
                    for (int py = 0; py < 4; py++) {
                        if (tetraminos[tetramino_atual][rotacionar(px, py, rotacao_atual)] != '.') {
                            grade_jogo[(y_atual + py) * LARGURA_JOGO + (x_atual + px)] = tetramino_atual + 1;
                        }
                    }
                }

                for (int py = 0; py < 4; py++) {
                    if (y_atual + py < ALTURA_JOGO - 1) {
                        int bLine = 1;
                        for (int px = 1; px < LARGURA_JOGO - 1; px++) {
                            if (grade_jogo[(y_atual + py) * LARGURA_JOGO + px] == 0) {
                                bLine = 0;
                                break;
                            }
                        }

                        if (bLine) {
                            for (int px = 1; px < LARGURA_JOGO - 1; px++) {
                                grade_jogo[(y_atual + py) * LARGURA_JOGO + px] = 8;
                            }
                            if (total_linhas < 4)
                                vetor_linhas[total_linhas++] = y_atual + py;
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

                x_atual = LARGURA_JOGO / 2 - 2;
                y_atual = 0;
                rotacao_atual = 0;
                tetramino_atual = rand() % 7;

                if (!pode_encaixar(tetramino_atual, rotacao_atual, x_atual, y_atual)) {
                    fim_jogo = 1;
                    screenGotoxy(INICIO_X, INICIO_Y + ALTURA_JOGO / 2);
                    printf("GAME OVER!");
                }
            }
        }

        for (int y = 0; y < ALTURA_JOGO; y++) {
            for (int x = 0; x < LARGURA_JOGO; x++) {
                screenGotoxy(INICIO_X + x, INICIO_Y + y);
                switch (grade_jogo[y * LARGURA_JOGO + x]) {
                    case 0: printf(" "); break;
                    case 9: printf("#"); break;
                    case 8: printf("="); break;
                    default: printf("X"); break;
                }
            }
        }

        for (int px = 0; px < 4; px++) {
            for (int py = 0; py < 4; py++) {
                if (tetraminos[tetramino_atual][rotacionar(px, py, rotacao_atual)] != '.') {
                    screenGotoxy(INICIO_X + x_atual + px, INICIO_Y + y_atual + py);
                    printf("X");
                }
            }
        }

        usleep(50 * 1000);
    }

    screenDestroy();
    free(grade_jogo);
    return 0;
}