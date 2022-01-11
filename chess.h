//
// Created by tanak on 18/10/2021.
//

#ifndef GMAE_CHESS_H
#define GMAE_CHESS_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class chess {
private:
    Texture t1, t2;
    Sprite P[32], bBoard;
    int tam = 56, n = 0, player;
    int board[8][8] =       //tabuleiro para posicionar inicialmente as peças
            {-1, -2, -3, -4, -5, -3, -2, -1,
             -6, -6, -6, -6, -6, -6, -6, -6,
             0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0,
             6, 6, 6, 6, 6, 6, 6, 6,
             1, 2, 3, 4, 5, 3, 2, 1};
    bool isMove = false;
    float dx = 0, dy = 0;
    Vector2i pos;
    Vector2f oldPos, newPos;
    string str, posicao;

public:
    chess(int val);

    virtual ~chess();

    Vector2f Coord(char a, char b);

    String notas(Vector2f);

    void setCoisas(RenderWindow &window, string mov);

    void apaga_pos();

    void move(string);

    int check();

    string setMove(bool);

    void loadPosition();

    void draw(RenderWindow &window);
};

#endif //GMAE_CHESS_H
