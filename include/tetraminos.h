#ifndef _TETRAMINOS_H_
#define _TETRAMINOS_H_

#include "tetris.h"
#include "mapa.h"

extern char *tetraminos[9];

void carregar_tetraminos();
int rotacionar(int x, int y, int rot);
int pode_encaixar(MAPA* mapa, int tetramino, int rot, int posX, int posY);
void explodir(MAPA* t, int cx, int cy);
void posicionar_tetramino_no_mapa(MAPA *t, int tipo, int rot, int x, int y);
int remover_linhas_completas(MAPA *t);
void exibir_prox_peca(int proxima_peca);
void exibir_linhas_removidas(int total_linhas_remov);

#endif