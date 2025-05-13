#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

typedef enum{
    ESQUERDA,
    DIREITA,
    BAIXO,
    CAIR,
    ROTACIONAR
} Movimentos;

void area_jogo(){
    int areaX = 8;
    int areaY = 4;
    int largura_tela = 18;
    int altura_tela = 18;

    for (int y= 0; y< altura_tela;y++){
        for (int x = 0;x < largura_tela; x++){
            screenGotoxy(areaX + x, areaY+y);

            if (x ==0 || x == largura_tela - 1){
                printf("|");
            } else {
                printf(" ");
            }
        }
    }

    for (int x =0; x <largura_tela; x++){
        screenGotoxy(areaX + x, areaY + altura_tela);
        printf("#");
    }
}

void banner_titulo(){
    int bannerX = (80 - 53) / 2;
    int bannerY = (24 - 6) / 2;

    screenGotoxy(bannerX, bannerY);
    printf(" _____ ______  _____  _____ ______  _____  _____ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("|_   _|| ___ \\|  ___||_   _|| ___ \\|_   _|/  ___|");
    screenGotoxy(bannerX, bannerY + 2);
    printf("  | |  | |_/ /| |__    | |  | |_/ /  | |  \\ `--. ");
    screenGotoxy(bannerX, bannerY + 3);
    printf("  | |  |    / |  __|   | |  |    /   | |   `--. \\");
    screenGotoxy(bannerX, bannerY + 4);
    printf("  | |  | |\\ \\ | |___   | |  | |\\ \\  _| |_ /\\__/ /");
    screenGotoxy(bannerX, bannerY + 5);
    printf("  \\_/  \\_| \\_|\\____/   \\_/  \\_| \\_| \\___/ \\____/ ");
}



int main(){
    /*------------------------- inicio do jogo -------------------------*/ 
    // titulo
    screenInit(1);
    banner_titulo();
    screenUpdate();
    sleep(2);
    system("clear");

    // incio da area do jogo
    screenInit(1);
    area_jogo();
    screenUpdate();
    sleep(5);
    timerInit(500000);

    /*------------------------- fim do jogo -------------------------*/ 
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}