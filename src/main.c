#include <stdio.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <unistd.h>

typedef enum{
    ESQUERDA,
    DIREITA,
    BAIXO,
    CAIR,
    ROTACIONAR
} Movimentos;

void area_jogo(){
    int areaX = 10;
    int areaY = 5;
    int largura_tela = 12;
    int altura_tela = 18;

    for (int y= 0; y< altura_tela;y++){
        for (int x = 0;x < largura_tela; x++){
            screenGotoxy(areaX + x, areaY+y);

            if (x ==0 || x == largura_tela -0){
                printf("#");
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

int main(){
    /*------------------------- inicio do jogo -------------------------*/ 
    screenInit(0);
    area_jogo();
    sleep(5);
    timerInit(50);






    /*------------------------- fim do jogo -------------------------*/ 
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}