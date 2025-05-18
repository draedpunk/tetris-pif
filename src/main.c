#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tetris.h"

#define LARGURA_JOGO 9
#define ALTURA_JOGO 18
#define INICIO_X 6
#define INICIO_Y 4

char *tetraminos[9];
unsigned char *grade_jogo = NULL;

void ler_mapa(MAPA *m) {
    FILE *f = fopen("mapa.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o mapa.\n");
        exit(1);
    }

    char linha[TAMANHO_MAX_COLUNAS + 2];
    int linhas = 0;
    int colunas = -1;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = 0;
        int len = strlen(linha);

        if (colunas == -1) colunas = len;
        if (len != colunas) {
            printf("Erro: mapa com colunas irregulares (linha %d).\n", linhas + 2);
            fclose(f);
            exit(1);
        }

        if (linhas >= TAMANHO_MAX_LINHAS) {
            printf("Erro: mapa excede o número máximo de linhas.\n");
            fclose(f);
            exit(1);
        }

        strcpy(m->matriz[linhas], linha);
        linhas++;
    }

    fclose(f);

    m->linhas = linhas;
    m->colunas = colunas;
}
void imprimir_mapa(MAPA* t) {
    for (int i = 0; i < t->linhas; i++) {
        screenGotoxy(INICIO_X, INICIO_Y + i);
        printf("%s", t->matriz[i]);
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
                    ".0.."
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

void explodir(int cx, int cy) {
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int px = cx + x;
            int py = cy + y;
            if (px >= 0 && px < LARGURA_JOGO && py >= 0 && py < ALTURA_JOGO) {
                grade_jogo[py * LARGURA_JOGO + px] = 0;
            }
        }
    }
}

void desenhar_mapa_com_peca(MAPA* t, int tetramino_atual, int rotacao_atual, int x_atual, int y_atual) {
    // Desenha o mapa linha a linha
    for (int y = 0; y < t->linhas; y++) {
        for (int x = 0; x < t->colunas; x++) {
            screenGotoxy(INICIO_X + x, INICIO_Y + y);

            // Verifica se a peça atual ocupa essa posição
            int caractere_peca = 0; // 0 significa não tem peça aqui

            // Verifica se o ponto (x, y) está dentro da área da peça (4x4)
            int rel_x = x - x_atual;
            int rel_y = y - y_atual;
            if (rel_x >= 0 && rel_x < 4 && rel_y >= 0 && rel_y < 4) {
                int pi = rotacionar(rel_x, rel_y, rotacao_atual);
                if (tetraminos[tetramino_atual][pi] != '.') {
                    caractere_peca = 1;
                }
            }

            if (caractere_peca) {
                // Desenha o caractere da peça (exemplo: 'A' + tetramino_atual)
                printf("%c", 'A' + tetramino_atual);
            } else {
                // Desenha o caractere do mapa
                // Se for espaço, desenha espaço mesmo
                printf("%c", t->matriz[y][x]);
            }
        }
        printf("\n");
    }
}

int main() {
    MAPA t;
    ler_mapa(&t);  // t é a variável do tipo MAPA que será usada em todo o programa

    srand(time(NULL));
    grade_jogo = (unsigned char *) calloc(t.linhas * t.colunas, sizeof(unsigned char));
    if (!grade_jogo) {
        printf("Erro na alocação de memória!\n");
        return 1;
    }

    carregar_tetraminos();

    // Inicializa grade_jogo com 0
    for (int y = 0; y < t.linhas; y++) {
        for (int x = 0; x < t.colunas; x++) {
            grade_jogo[y * t.colunas + x] = 0;
        }
    }

    screenInit(1);

    int tetramino_atual = rand() % 9;
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

    timerInit(velocidade);

    while (!fim_jogo) {
        cair = timerTimeOver();
        ler_input(teclas);

        if (teclas[0] && pode_encaixar(&t, tetramino_atual, rotacao_atual, x_atual + 1, y_atual)) x_atual++;
        if (teclas[1] && pode_encaixar(&t, tetramino_atual, rotacao_atual, x_atual - 1, y_atual)) x_atual--;
        if (teclas[2] && pode_encaixar(&t, tetramino_atual, rotacao_atual, x_atual, y_atual + 1)) {
            y_atual++;
            timerUpdateTimer(velocidade);
        }
        if (teclas[3]) {
            if (bRotateHold && pode_encaixar(&t, tetramino_atual, rotacao_atual + 1, x_atual, y_atual))
                rotacao_atual++;
            bRotateHold = 0;
        } else {
            bRotateHold = 1;
        }

        int pode_descer = pode_encaixar(&t, tetramino_atual, rotacao_atual, x_atual, y_atual + 1);

        if (cair && pode_descer) {
            y_atual++;
            timerUpdateTimer(velocidade);
        } else if (!pode_descer) {
            tetraminos_jogados++;

            if (tetraminos_jogados % 50 == 0 && velocidade >= 100) {
                velocidade -= 50;
                timerUpdateTimer(velocidade);
            }

            for (int py = 0; py < 4; py++) {
                for (int px = 0; px < 4; px++) {
                    int pi = rotacionar(px, py, rotacao_atual);
                    if (tetraminos[tetramino_atual][pi] != '.') {
                        int mx = x_atual + px;
                        int my = y_atual + py;
                        if (mx >= 0 && mx < t.colunas && my >= 0 && my < t.linhas) {
                            t.matriz[my][mx] = 'A' + tetramino_atual;
                        }
                    }
                }
            }

            if (tetramino_atual == 8) {
                explodir(x_atual + 1, y_atual + 1);
            }

            total_linhas = 0;
            for (int py = 0; py < 4; py++) {
                int linha = y_atual + py;
                if (linha < t.linhas - 1) {
                    int cheia = 1;
                    for (int px = 1; px < t.colunas - 1; px++) {
                        if (grade_jogo[linha * t.colunas + px] == 0) {
                            cheia = 0;
                            break;
                        }
                    }
                    if (cheia) {
                        for (int px = 1; px < t.colunas - 1; px++) {
                            grade_jogo[linha * t.colunas + px] = 8;
                        }
                        if (total_linhas < 4) {
                            vetor_linhas[total_linhas++] = linha;
                        }
                    }
                }
            }

            if (tetramino_atual != 8) {
                pontuacao += 25;

                if (total_linhas > 0) {
                    pontuacao += (1 << total_linhas) * 100;

                    for (int i = 0; i < total_linhas; i++) {
                        for (int y = vetor_linhas[i]; y > 0; y--) {
                            for (int x = 1; x < t.colunas - 1; x++) {
                                grade_jogo[y * t.colunas + x] = grade_jogo[(y - 1) * t.colunas + x];
                            }
                        }
                    }
                    total_linhas = 0;
                }
            }

            tetramino_atual = rand() % 9;
            rotacao_atual = 0;
            x_atual = LARGURA_JOGO / 2 - 2;
            y_atual = 0;

            if (!pode_encaixar(&t, tetramino_atual, rotacao_atual, x_atual, y_atual)) {
                fim_jogo = 1;
                screenGotoxy(INICIO_X, INICIO_Y + t.linhas / 2);
                printf("GAME OVER!");
            }
        }

        desenhar_mapa_com_peca(&t, tetramino_atual, rotacao_atual, x_atual, y_atual);

        screenGotoxy(INICIO_X, INICIO_Y + t.linhas + 1);
        printf("Pontuacao: %d", pontuacao);

        screenUpdate();
        usleep(50000);
    }

    screenDestroy();
    free(grade_jogo);
    return 0;
}
