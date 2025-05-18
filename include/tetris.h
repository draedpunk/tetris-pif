#ifndef _TETRIS_H_
#define _TETRIS_H_

#define TAMANHO_MAX_LINHAS 24
#define TAMANHO_MAX_COLUNAS 40

typedef struct {
    char matriz[TAMANHO_MAX_LINHAS][TAMANHO_MAX_COLUNAS];
    int linhas;
    int colunas;
} MAPA;

void carregar_tetraminos();
int rotacionar(int x, int y, int rot);
int pode_encaixar(MAPA* mapa, int tetramino, int rot, int posX, int posY);
void desenhar_area_jogo();
void ler_input(int tecla[4]);
void explodir(int cx, int cy);
void banner_titulo();
void exibir_banner();

#endif