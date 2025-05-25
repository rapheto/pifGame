#include "player.h"
#include <string.h>

static Player player; //Struct jogador definida em Player.h

//Inicia os dados do player
void initPlayer(const char *nickname) {
    player.vidas = 5;
    player.pontos = 0;     
    strncpy(player.nickname, nickname, MAX_NICKNAME_LENGTH - 1);
    player.nickname[MAX_NICKNAME_LENGTH - 1] = '\0';
}

//Usada em outras outros scripts para buscar os dados do player
Player* getPlayer() { 
    return &player;
}

//Altera a quantidade de vidas
void setVidas(int vidas) {
    player.vidas = vidas;
}

//Altera a quantidade de pontos
void setPontos(int pontos) {
    player.pontos = pontos;
}

//Seta o nickname do jogador depois de cria-lo no inicio
void setNickname(const char *nickname) {
    strncpy(player.nickname, nickname, MAX_NICKNAME_LENGTH - 1); //Função de copiar
    player.nickname[MAX_NICKNAME_LENGTH - 1] = '\0';
}