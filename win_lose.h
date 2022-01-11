#ifndef GMAE_WIN_LOSE_H
#define GMAE_WIN_LOSE_H

#include <SFML/Graphics.hpp>

using namespace sf;
class win_lose { //classe feita somente para poder fazer o load e desenhas as sprites das imagens win e lose
private:
    Sprite win, lose;
    Texture winT, loseT;
public:
    win_lose();
    void draw(RenderWindow &window, int);
};


#endif //GMAE_WIN_LOSE_H
