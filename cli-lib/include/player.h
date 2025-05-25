#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NICKNAME_LENGTH 20

typedef struct {
    int vidas;
    int pontos;
    char nickname[MAX_NICKNAME_LENGTH];
} Player;

void initPlayer(const char *nickname);

Player* getPlayer();

void setVidas(int vidas);

void setPontos(int pontos);

void setNickname(const char *nickname);

#endif