//
// Created by hirata on 09/11/20.
//

#ifndef FORCA4_BUTTUN_H
#define FORCA4_BUTTUN_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Buttun {
private:
    Sprite play;
    Texture playT;
    Font font;
    Text texto, blitz;
    int player; //para saber se esta lidando com o cliente(1) ou servidor(2)
public:
    Buttun(int);

    ~Buttun();

    //Functions
    bool isMouseOver(RenderWindow &window);
    void draw(RenderWindow &window);
};


#endif //FORCA4_BUTTUN_H
