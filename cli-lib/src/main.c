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

//Operadores
char operadores[][4] = {"∧", "∨", "→", "↔"};

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

typedef struct{
    int vidas;
    int pontos;
    char nickname[20];
} Player;

Asteroide asteroides[MAX_ASTEROIDES];


//Gerador de proposições
char negar(char valor) {
    return (valor == 'V') ? 'F' : 'V';
}

void gerarProposicaoSimples(Proposicao *prop)
{
    char A = (rand() % 2) ? 'V' : 'F';
    char B = (rand() % 2) ? 'V' : 'F';

    int negaA = rand() % 2;
    int negaB = rand() % 2;

    char valA = negaA ? negar(A) : A;
    char valB = negaB ? negar(B) : B;

    int opIndex = rand() % 4;
    char *operador = operadores[opIndex];

    // Monta a string da expressão
    char exprA[4], exprB[4];
    sprintf(exprA, "%s%c", negaA ? "¬" : "", A);
    sprintf(exprB, "%s%c", negaB ? "¬" : "", B);
    sprintf(prop->valor, "(%s %s %s)", exprA, operador, exprB);

    // Avalia a expressão
    char resultado;

    if (strcmp(operador, "∧") == 0)
        resultado = (valA == 'V' && valB == 'V') ? 'V' : 'F';
    else if (strcmp(operador, "∨") == 0)
        resultado = (valA == 'F' && valB == 'F') ? 'F' : 'V';
    else if (strcmp(operador, "→") == 0)
        resultado = (valA == 'V' && valB == 'F') ? 'F' : 'V';
    else if (strcmp(operador, "↔") == 0)
        resultado = (valA == valB) ? 'V' : 'F';

    prop->resposta[0] = resultado;
    prop->resposta[1] = '\0';
}


//Colisores
void verificarColisaoBala(Player *playerPontos)
{
    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        if (asteroides[i].ativo)
        {
            // Verificar colisão na mesma coluna e linha
            if ((bulletX >= asteroides[i].x && bulletX <= asteroides[i].x + 2) &&
                bulletY == asteroides[i].y)
            {
                if (asteroides[i].tipo == 0)
                {
                    
                    printf("Acertou V\n");
                    playerPontos->pontos += 100;
                }
                else 
                {
                    playerPontos->pontos -= 10;
                    printf("Acertou F\n");   
                }

                // Remover o  meteoro e a bala
                asteroides[i].ativo = 0;
                screenGotoxy(asteroides[i].x, asteroides[i].y);
                printf("   ");

                screenGotoxy(bulletX, bulletY);
                printf("  ");

                isShooting = 0;
                break;
            }
        }
    }
}

void verificarColisaoJogador(Player *player)
{
    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        if (asteroides[i].ativo)
        {
            if (asteroides[i].y == y &&
                asteroides[i].x >= x && asteroides[i].x <= x + 4)
            {
                player->vidas -= 1;
                asteroides[i].ativo = 0;

                screenGotoxy(asteroides[i].x, asteroides[i].y);
                printf("   ");
                screenUpdate();

                // Você pode adicionar som, ou tela piscando aqui se quiser
            }
        }
    }
}


//Cenário
void caixaLogica(Proposicao expressao, Player playerStats)
{
    screenGotoxy(3, 3);
    printf("EXPRESSÃO: %s", expressao.valor);
    screenGotoxy(2, 5);
    
    //Bordas
    printf("_");
    screenGotoxy(MAXX*0.5, 2);
    printf("|");
    screenGotoxy(MAXX*0.5, 3);
    printf("|");
    screenGotoxy(MAXX*0.5, 4);
    printf("|");

    screenGotoxy((MAXX*0.5) + 8, 3);
    printf("VIDAS: %d            PONTUAÇÃO: %04d", playerStats.vidas, playerStats.pontos);

}


//Player
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


//Asteroides ou Inimigos
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
                    printf(" V ");
                else
                    printf(" F ");
            }
        }
    }
}

int main()
{
    static int ch = 0;
    static long timer = 0;

    //Player
    Player player;
    player.vidas = 3;
    player.pontos = 0;

    Proposicao propAtual;
    gerarProposicaoSimples(&propAtual);

    srand(time(NULL)); // Inicializa números aleatórios

    screenInit(1);
    keyboardInit();
    timerInit(33);
    caixaLogica(propAtual, player);

    while (ch != 10 && timer <= 1000)
    {
        caixaLogica(propAtual, player);
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
                verificarColisaoBala(&player);
            }

            if (rand() % 10 == 0)
            {
                criarAsteroide();
            }

            atualizarAsteroides();

            verificarColisaoJogador(&player);

            screenUpdate();
            timer++;
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}