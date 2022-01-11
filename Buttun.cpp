//
// Created by hirata on 09/11/20.
//

#include "Buttun.h"

Buttun::Buttun(int val) : player(val) {
    if (player == 1) { //tela inicial do cliene um simples imagem e string
        playT.loadFromFile("../imagens/play.png");
        play.setTexture(playT);
        this->play.setPosition(120, 80);
        font.loadFromFile("../src/BodoAmat.ttf");
        texto.setFont(font);
        texto.setString("    Blitz Chess\n\nclique em play\npara jogar.\nVoce joga com\nas brancas.");
        texto.setCharacterSize(35);
        texto.setFillColor(Color::White);
        texto.setPosition(70, 160);
    }
    if (player == 2) { //aqui foi utilizado somente uma string para tela inicial do servidor
        font.loadFromFile("../src/BodoAmat.ttf");
        texto.setFont(font);
        texto.setString("   Blitz Chess\n\nAguardando\nOponente...\n\nVoce e o servidor.\njoga com as\ndouradas.");
        texto.setCharacterSize(35);
        texto.setFillColor(Color::White);
        texto.setPosition(70, 70);
    }
}

Buttun::~Buttun() {

}

bool Buttun::isMouseOver(RenderWindow &window) { //funçao que determina se o click do mouse
    float mouseX = Mouse::getPosition(window).x;    //foi feito sobre a imagem play
    float mouseY = Mouse::getPosition(window).y;

    float btnPosX = this->play.getPosition().x;
    float btnPosY = this->play.getPosition().y;

    float btnXposWidth = this->play.getPosition().x + this->play.getLocalBounds().width;
    float btnYposHeight = this->play.getPosition().y + this->play.getLocalBounds().height;

    if (mouseX < btnXposWidth && mouseX > btnPosX && mouseY < btnYposHeight && mouseY > btnPosY) return true;
    else return false;
}

void Buttun::draw(RenderWindow &window) { //desenha as sprites.
    if (player == 2) window.draw(texto);
    if (player == 1) {
        window.draw(this->play);
        window.draw(texto);
    }
}
