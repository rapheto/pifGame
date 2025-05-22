#include "player.h"
#include <string.h>

static Player player;

void initPlayer(const char *nickname) {
    player.vidas = 5;
    player.pontos = 0;     
    strncpy(player.nickname, nickname, MAX_NICKNAME_LENGTH - 1);
    player.nickname[MAX_NICKNAME_LENGTH - 1] = '\0';
}

Player* getPlayer() {
    return &player;
}

void setVidas(int vidas) {
    player.vidas = vidas;
}

void setPontos(int pontos) {
    player.pontos = pontos;
}

void setNickname(const char *nickname) {
    strncpy(player.nickname, nickname, MAX_NICKNAME_LENGTH - 1);
    player.nickname[MAX_NICKNAME_LENGTH - 1] = '\0';
}