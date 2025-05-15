/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MAX_ASTEROIDES 10

float x = 48, y = 33; // Posição Inicial do jogador
int incX = 1;         // Tamanho da movimentação do jogador

// Bool
int isShooting = 0;

// Bala
int bulletSpeed = 1;
int bulletX = 50;
int bulletY = 31;

typedef struct
{
    char valor[20];
    char resposta[2];
} Proposicao;

typedef struct
{
    int x;
    int y;
    int ativo;
    int tipo; // Tipo para animação
} Asteroide;

Asteroide asteroides[MAX_ASTEROIDES];

void caixaLogica(Proposicao expressao)
{
    screenGotoxy(3, 3);
    printf("EXPRESSÃO: %s", expressao.valor);
    screenGotoxy(2, 5);
    printf("-------------------------------------------------------------------------------------------------");
}

void movimentacao(int ch)
{
    screenGotoxy(x, y);
    printf("      "); // Apaga o jogador anterior

    if (ch == 100 && x < MAXX - strlen("<I--I>") - 1)
    {
        x += incX;
    }
    else if (ch == 97 && x > MINX + 1)
    {
        x -= incX;
    }

    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(x, y);
    printf("<I--I>");
    screenUpdate();
}

void bullet()
{
    screenGotoxy(bulletX, bulletY + bulletSpeed);
    printf("  "); // Apaga a bala anterior

    screenGotoxy(bulletX, bulletY);
    printf("||"); // Cria a bala

    bulletY -= bulletSpeed;

    if (bulletY <= MINY + 4)
    {
        screenGotoxy(bulletX, bulletY + bulletSpeed);
        printf("  ");
        isShooting = 0;
    }

    screenUpdate();
}

void atirar()
{
    isShooting = 1;
    bulletX = x + 2;
    bulletY = y - 2;
}

void criarAsteroide()
{
    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        if (!asteroides[i].ativo)
        {
            asteroides[i].ativo = 1;
            asteroides[i].x = rand() % (MAXX - 10) + 5;
            asteroides[i].y = 6;
            asteroides[i].tipo = rand() % 2; // Alterna entre dois tipos
            break;
        }
    }
}

int asteroidTick = 0;
void atualizarAsteroides()
{
    asteroidTick++;
    if (asteroidTick % 3 != 0)
        return; // só move a cada 3 atualizações (ajuste esse valor)

    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        if (asteroides[i].ativo)
        {
            screenGotoxy(asteroides[i].x, asteroides[i].y);
            printf("   ");

            asteroides[i].y++;

            if (asteroides[i].y >= MAXY - 1)
            {
                asteroides[i].ativo = 0;
            }
            else
            {
                screenGotoxy(asteroides[i].x, asteroides[i].y);
                if (asteroides[i].tipo == 0)
                    printf(" @ ");
                else
                    printf(" * ");
            }
        }
    }
}

int main()
{
    static int ch = 0;
    static long timer = 0;

    Proposicao propAtual;
    strcpy(propAtual.valor, "(V ∧ ¬F)");
    strcpy(propAtual.resposta, "V");

    srand(time(NULL)); // Inicializa números aleatórios

    screenInit(1);
    keyboardInit();
    timerInit(33);
    caixaLogica(propAtual);

    while (ch != 10 && timer <= 1000)
    {
        if (keyhit())
        {
            ch = readch();

            if (ch == 32 && !isShooting)
            {
                atirar();
            }
            else
            {
                movimentacao(ch);
            }
        }

        if (timerTimeOver() == 1)
        {
            if (isShooting)
            {
                bullet();
            }

            if (rand() % 10 == 0)
            {
                criarAsteroide();
            }

            atualizarAsteroides();

            screenUpdate();
            timer++;
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
