#ifndef _TETRIS_H_
#define _TETRIS_H_

#include "mapa.h"
#include "tetraminos.h"

void ler_input(int tecla[4]);
//void atualizar_pontuacao(int *pontuacao, int linhas, int eh_explosiva);
void processar_input(int *teclas, int *x, int *y, int *rot, int *bRotateHold, MAPA *t, int tipo);
int verificar_game_over(MAPA *t, int tipo, int rot, int x, int y);

#endif