#include "win_lose.h"

win_lose::win_lose() {
    winT.loadFromFile("../imagens/win.png");
    win.setTexture(winT);
    win.setPosition(10, 10);
    loseT.loadFromFile("../imagens/lose.png");
    lose.setTexture(loseT);
    lose.setPosition(10, 10);
}

void win_lose::draw(RenderWindow &window, int val) {
    if(val == 1) window.draw(win);
    if (val == 2) window.draw(lose);
}
