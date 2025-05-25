#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "menu.h"
#include "player.h"
#include "ranking.h"

#define MAX_ASTEROIDES 10

float x, y; //Posição do jogador
int incX = 1; // Tamanho da movimentação do jogador

// Bool da bala
int isShooting = 0;

// Bala
int bulletSpeed = 1; //Pixels que irá mover / "velocidade"
int bulletX, bulletY; //Posição da bala



typedef struct
{
    char valor[20];
    char resposta;
} Proposicao; //Struct para a proposição que será criada com um valor e uma resposta

typedef struct
{
    int x;
    int y;
    int ativo;
    int tipo;
} Asteroide; //Struct para o asteroide que terá uma posição, um validador de existencia e um tipo (V ou F)

//Define a quantidade máxima de asteroides == 10
Asteroide asteroides[MAX_ASTEROIDES];



//Operadores lógicos para o printf
char operadores[][4] = {"∧", "∨", "→", "↔"};

//Função para negar o valor A ou B da proposição
char negar(char valor) {
    //(condição) ? valor_se_verdadeiro : valor_se_falso;
    return (valor == 'V') ? 'F' : 'V';
}

//Gerador de proposições
void gerarProposicaoSimples(Proposicao *prop)
{
    //(condição) ? valor_se_verdadeiro (1) : valor_se_falso (0); 
    //Divide um número aleatório por 2 e a sobra define se ele é V ou F
    //Sobra = 1 (V) Sobra = 0 (F)
    char A = (rand() % 2) ? 'V' : 'F'; 
    char B = (rand() % 2) ? 'V' : 'F';

    //Divide um número aleatório por 2 e a sobra define se ele é Negado ou não
    int negaA = rand() % 2;
    int negaB = rand() % 2;

    //Se negaA/B == 1 ele inverte o valor (Nega)
    //Se negaA/B == 0 o valor é mantido (Não nega)
    char valA = negaA ? negar(A) : A;
    char valB = negaB ? negar(B) : B;

    //Define qual vai ser o operador lógico
    int opIndex = rand() % 4;
    char *operador = operadores[opIndex];

    // Monta a expressão
    char exprA[4], exprB[4];
    //Altera o texto caso seja negada ou não
    sprintf(exprA, "%s%c", negaA ? "¬" : "", A); 
    sprintf(exprB, "%s%c", negaB ? "¬" : "", B);
    //Mostra a expressão finalizada
    sprintf(prop->valor, "(%s %s %s)", exprA, operador, exprB);

    // Avalia a expressão
    char resultado;

    if (strcmp(operador, "∧") == 0) //Operador lógico "E" (as duas proposições tem que ser verdadeiras)
        //Só será V se os dois forem V, se não será F
        resultado = (valA == 'V' && valB == 'V') ? 'V' : 'F';

    else if (strcmp(operador, "∨") == 0) //Operador lógico "OU" (As duas tem que ser falsas)
        //Só será F se os dois forem F, se não será V
        resultado = (valA == 'F' && valB == 'F') ? 'F' : 'V';

    else if (strcmp(operador, "→") == 0) //Operador lógico "ENTÃO" 
        //Só será F quando A = V e B = F, todo o resto é verdadeiro
        resultado = (valA == 'V' && valB == 'F') ? 'F' : 'V';

    else if (strcmp(operador, "↔") == 0) //Operador lógico "SE E SOMENTE SE"
        //Os casos iguais são sempre V, o resto são falsos
        resultado = (valA == valB) ? 'V' : 'F';

    //Seta o valor da resposta com o do resultado (será utilizado na colisão)
    prop->resposta = resultado;
}



//Colisores
//Colisor da bala com as respostas
void verificarColisaoBala(Proposicao *prop)
{
    Player *playerPontos = getPlayer(); //Pega a pontuação do player

    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        if (asteroides[i].ativo) //Verifica se o asteroide existe
        {
            // Verificar colisão na mesma coluna e linha
            if (bulletX == asteroides[i].x && bulletY == asteroides[i].y) //Funciona mas com um bug
            {
                //(condição) ? valor_se_verdadeiro (1) : valor_se_falso (0); 
                //Define o valor da resposta baseado na resposta do asteroide
                int respostaEsperada = (prop->resposta == 'V') ? 0 : 1; //Converte o meteoro V e F em 0 ou 1

                if (asteroides[i].tipo == respostaEsperada) //Caso acerte
                {
                    setPontos(playerPontos->pontos += 100); //Pontua o jogador caso ele acerte
                    gerarProposicaoSimples(prop); //Gera outra proposição
                }
                else //Caso erre
                {
                    setVidas(playerPontos->vidas -= 1); //Tira vida do jogador
                    setPontos(playerPontos->pontos -= 50); //Tira a pontuação
                    gerarProposicaoSimples(prop); //Gera outra proposição
                }

                // Remover o  meteoro e a bala
                asteroides[i].ativo = 0;
                screenGotoxy(asteroides[i].x, asteroides[i].y);
                printf(" ");

                //Remove a bala da tela
                screenGotoxy(bulletX, bulletY);
                printf(" ");
                screenGotoxy(bulletX, bulletY + bulletSpeed);
                printf(" ");
                bulletX = -1;
                bulletY = -1;

                //Zera os bloqueadores
                isShooting = 0;
                asteroides[i].ativo = 0;

                //Atualiza a tela e encerra
                screenUpdate();
                break;
            }
        }
        
    }
}

//Colisor do jogador
void verificarColisaoJogador()
{
    Player *player = getPlayer(); //Pega o player
    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        if (asteroides[i].ativo)
        {
            if (asteroides[i].y == y && asteroides[i].x >= x && asteroides[i].x <= x + 4) //Compara se a posição do jogador é a mesma que a do asteroide
            {
                setVidas(player->vidas -= 1); //Tira uma vida
                asteroides[i].ativo = 0; //Desativa o asteroide

                //Apaga o asteroide
                screenGotoxy(asteroides[i].x, asteroides[i].y); 
                printf(" ");

                //Atualiza a tela
                screenUpdate();
            }
        }
    }
}



//Cenário do jogo
void caixaLogica(Proposicao expressao)
{
    Player *playerStats = getPlayer(); //Pega o player
    //Mostra a expressão lógica na tela
    screenGotoxy(3, 3);
    printf("EXPRESSÃO: %s    RES:%c  ", expressao.valor, expressao.resposta); 
    
    //Bordas
    screenGotoxy(2, 5);
    printf("_________________________________________________________________________________________________");
    screenGotoxy(MAXX*0.5, 2);
    printf("|");
    screenGotoxy(MAXX*0.5, 3);
    printf("|");
    screenGotoxy(MAXX*0.5, 4);
    printf("|");

    //Mostra os status do jogador na tela
    screenGotoxy((MAXX*0.5) + 8, 3);
    printf("VIDAS: %d            PONTUAÇÃO: %04d", playerStats->vidas, playerStats->pontos);

}



//Player
void movimentacao(int ch)
{
    // Apaga a posição anterior do jogador 
    screenGotoxy(x, y);
    printf("      "); 

    if (ch == 100 && x < MAXX - strlen("<I-I>") - 1) //Tecla A
    {
        x += incX; //Move para a esquerda
    }
    else if (ch == 97 && x > MINX + 1) // Tecla D
    {
        x -= incX; //Move para a direita
    }

    //Cria o jogador na nova posição
    screenGotoxy(x, y);
    printf("<I-I>");

    //Atualiza a tela
    screenUpdate();
}

void bullet()
{
    // Apaga a bala na posição anterior
    screenGotoxy(bulletX, bulletY + bulletSpeed);
    printf(" "); 

    // Cria a bala na nova posição
    screenGotoxy(bulletX, bulletY);
    printf("|"); 

    //Faz a bala subir
    bulletY -= bulletSpeed;
    //Caso a bala vá para a borda da tela
    if (bulletY <= MINY + 4)
    {
        //Apaga a bala da tela
        screenGotoxy(bulletX, bulletY + bulletSpeed);
        printf(" ");
        isShooting = 0;
    }

    //Atualiza a tela
    screenUpdate();
}

//Função de atirar
void atirar()
{
    //Trava a bala de ser atirada mais de uma vez
    isShooting = 1;
    //Define a posição inicial dela
    bulletX = x + 2;
    bulletY = y - 2;
}



//Asteroides ou Inimigos
void criarAsteroide()
{
    for (int i = 0; i < MAX_ASTEROIDES; i++) //Não deixa criar mais de 10 asteroides na tela simultaneamente
    {
        if (!asteroides[i].ativo)
        {
            asteroides[i].ativo = 1; //Ativa ele
            asteroides[i].x = rand() % (MAXX - 10) + 5; //Define uma posição aleatória para X
            asteroides[i].y = 6;
            asteroides[i].tipo = rand() % 2; //Define o tipo (V ou F)
            break;
        }
    }
}

//Mover o asteroide na tela
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
            //Apaga a posição anterior dele
            screenGotoxy(asteroides[i].x, asteroides[i].y);
            printf("   ");

            //Atualiza o Y para ele descer
            asteroides[i].y++;

            //Desativa ele caso toque na borda de baixo
            if (asteroides[i].y >= MAXY - 1)
            {
                asteroides[i].ativo = 0;
            }
            else //Se não tocar na borda
            {
                //Cria ele na nova posição de acordo com o tipo
                screenGotoxy(asteroides[i].x, asteroides[i].y);
                if (asteroides[i].tipo == 0)
                    printf("V");
                else
                    printf("F");
            }
        }
    }
}



//Cria o jogo
void inicializarJogo() {
    Proposicao prop; //Variavel do tipo proposição
    gerarProposicaoSimples(&prop); //Gera a primeira espressão lógica

    Player *player = getPlayer(); //Cria e pega os dados do player
    setVidas(5); //Seta a vida inicial
    setPontos(0); //Pontuação inicial

    static int ch = 0; //Caracter inicial é 0 
    static long timer = 0; //Timer inicial é 0

    //Zera a os asteroides na tela
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        asteroides[i].ativo = 0;
    }

    // Posição Inicial do jogador
    x = 48;
    y = 33; 

    // Posição Inicial da bala e zerar bloqueador
    bulletX = 50;
    bulletY = 31;
    isShooting = 0;

    srand(time(NULL)); // Inicializa números aleatórios
    screenInit(1); //Inicia a tela
    keyboardInit(); //Inicia o teclado
    timerInit(50); //Define a taxa de atualizações do jogo (30FPS)

    caixaLogica(prop); //Cria a caixa no cenário
    movimentacao(100); //Cria o jogador na tela
    screenUpdate(); //Atualiza a tela

    while (player->vidas > 0) { //Roda o jogo até o jogador morrer
        caixaLogica(prop); //Sempre atualiza a caixa lógica com o valor atual

        if (keyhit()) { //Sempre identifica qual tecla é pressionada
            int ch = readch();
            if (ch == 32 && !isShooting) {  // Tecla espaço
                atirar();
            }
            else{
                movimentacao(ch); //A ou D movem
            }
        }

        if (timerTimeOver() == 1) //Timer para identificar
        {
            if (isShooting) //Sempre spawna a bala caso seja verdadeiro
            {
                bullet();
                verificarColisaoBala(&prop);
            }

            if (rand() % 10 == 0) //Cria os asteroides
            {
                criarAsteroide();
            }

            atualizarAsteroides(); //Sempre tualiza a posição deles na tela

            verificarColisaoJogador(); //Sempre verifica a colisão

            screenUpdate(); //Sempre atualiza a tela
            timer++;
        }
    }
    salvarPontuacao(); //Salva pontos atuais da ultima partida
    telaDerrota(); //Cria a tela de derrota
}


int main()
{
    screenSetColor(WHITE, BLACK); //Muda a cor da tela
    iniciarMenu(); //Inicia tudo a partir do menu
    return 0;
}