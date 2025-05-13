/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

float x = 48, y = 28; //Posicao Inicial do jogador

int incX = 1; //Tamanho da movimentacao do jogador

//Bool
int isShooting = 0;

int bulletSpeed = 1;
int bulletX = 0;
int bulletY = 0;

void movimentacao(int ch){
    screenGotoxy(x, y); //vai para posicao inicial
    printf("      "); //Apaga o objeto
    /*if (ch == 119 && y > MINY + 1)
    {
        y = y - 0.5;
    }
    else if (ch == 115 && y < MAXY - 1)
    {
        y++;
    }*/
    if (ch == 100 && x < MAXX - strlen("<I--I>") - 1) //Move direita
    {
        x = x + incX;
    }
    else if (ch == 97 && x > MINX + 1) //Move esquerda
    {
        x = x - incX;
    }
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(x, y); //Objeto vai para a nova posicao
    printf("<I--I>"); //Crio o objeto
    screenUpdate(); //Atualiza a tela
}

void bullet(){
    screenGotoxy(bulletX, bulletY + bulletSpeed); //Nao deixa criar varias balas na tela
    printf("    "); //Apaga 

    screenGotoxy(bulletX, bulletY); //vai para a posicao do jogador
    printf("||"); //Cria

    screenUpdate(); //Atualiza a tela

    bulletY -= bulletSpeed; //Faz a bala subir

    if (bulletY <= MINY + 2) { //Destruir a bala caso ela chegue no limite
        screenGotoxy(bulletX, bulletY + bulletSpeed); //Faz a bala não travar na borda
        printf("  "); //Destroi ela
        isShooting = 0; //Permitir atirar de novo
    }

}

void atirar(){ //Arma
    isShooting = 1; //Bloquear atirar de novo
    bulletX = (int)x + 2; //Pega a posicao do meio do jogador
    bulletY = (int)y - 2; //Coloca ele acima do jogador
}

int main() 
{
    static int ch = 0;
    static long timer = 0;
    
    screenInit(1);
    keyboardInit();
    timerInit(33); //Atualiza o jogo a cada tantos milisegundos (1000ms / 33ms = 30 FPS)
    screenUpdate();
    
    while (ch != 10 && timer <= 1000)
    {
        if (keyhit()) { //funcao de identificar quando uma tecla é pressionada
            ch = readch(); // readch() le a tecla clicada pelo jogador
    
            //Habilidades do jogador
            //Atirar
            if (ch == 32 && !isShooting) {
                atirar();
            } 
            //Movimentar
            else {
                movimentacao(ch);
            }
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            bullet();
            screenUpdate();
            timer++;
        }
    }

    //Encerra o jogo
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
