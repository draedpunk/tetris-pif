#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tetris.h"
#include "tetraminos.h"
#include "mapa.h"
#include "pontuacao.h"

// limite maximo de jogadores pro jogo 50
// ranking vai mostrar no max as 5 maiores pontuacoes

void atualizar_pontuacao(int *pontuacao, int linhas, int eh_explosiva) {
    if (eh_explosiva){
        *pontuacao -= 50;  
        return; 
    }
    *pontuacao += 25;
    if (linhas > 0) {
        *pontuacao += linhas * 75; 
    }
}


void receber_dados_pontuacao(Jogador pontos[], int *qtd_dados){
    FILE *arquivo_pontos = fopen("./assets/ascii-arts", "r");
    if (arquivo_pontos == NULL){
        printf("erro ao abrir arquivo.\n");
        return;
    }

    *qtd_dados =0;
    while(fscanf(arquivo_pontos, "%25[^|] | %d\n", pontos[*qtd_dados].nome, &pontos[*qtd_dados].pontuacao) == 2){
        (*qtd_dados)++;

        if ((*qtd_dados) >= LIMITE_JOGADORES){
            break;
        }

    }
    fclose(arquivo_pontos);
}

void colocar_ordem_cresc(Jogador pontos[], int qtd_dados){
    Jogador valor_temporario;

    for (int i =0; i < qtd_dados-1; i++){
        for (int j= i +1; j <qtd_dados; j++){

            if(pontos[i].pontuacao < pontos[j].pontuacao){
                valor_temporario = pontos[i];
                pontos[i] = pontos[j];
                pontos[j] = valor_temporario;
            }
        }
    }
}

void exibir_ranking(){
    Jogador pontos[LIMITE_JOGADORES];
    int cont_jogadores;
    int maior_pontuacao;
    receber_dados_pontuacao(pontos, &cont_jogadores);
    screenClear();

    screenGotoxy(SCRSTARTX + 35, SCRSTARTY + 5);
    printf("RANKING\n");

    if (cont_jogadores ==0){
        screenGotoxy(SCRSTARTX + 29, SCRSTARTY + 12);
        printf("Nenhuma pontuacao disponivel.\n");
    } else{
        colocar_ordem_cresc(pontos, cont_jogadores);
        maior_pontuacao = cont_jogadores < TOP_MAIORES_PONT ? cont_jogadores : TOP_MAIORES_PONT;

        for (int i =0; i < maior_pontuacao; i++){
            screenGotoxy(SCRSTARTX + 29, SCRSTARTY + 12 + i);

            char dados_pontuacao[60];

            if (i ==0){
                snprintf(dados_pontuacao, sizeof(dados_pontuacao), "%d. ★ %s - %d pontos", i+1, pontos[i].nome, pontos[i].pontuacao);
            } else{
                snprintf(dados_pontuacao, sizeof(dados_pontuacao), "%d. ★ %s - %d pontos", i+1, pontos[i].nome, pontos[i].pontuacao);
            }

            // centralizar ranking de alguma forma aq
        }
    }

    screenGotoxy(SCRSTARTX + 20, SCRSTARTY + 14 + (maior_pontuacao > 0 ? maior_pontuacao : 1));
    printf("Pressione qualquer tecla para voltar ao menu principal.\n");
    readch();

}

void exibir_pontuacao(int *pontuacao){
    screenGotoxy(INICIO_X + LARGURA_JOGO+ 9, INICIO_Y +12);
    printf("+---Pontos---+");

    for(int i =0; i<1; i++){
        screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y +13);
        printf("|            |");
    }
    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 14);
    printf("+------------+");

    screenGotoxy(INICIO_X+LARGURA_JOGO + , INICIO_Y + 13);
    printf("%4d", *pontuacao);
}

