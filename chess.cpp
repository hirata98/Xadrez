//
// Created by tanak on 18/10/2021.
//
#include "chess.h"

chess::chess(int val) {
    player = val; //para saber se esta lidando com o cliente(1) ou servidor(2)
    t1.loadFromFile("../imagens/figures.png");
    t2.loadFromFile("../imagens/board.png");
    bBoard.setTexture(t2);
    for (int i = 0; i < 32; ++i) P[i].setTexture(t1);

}

chess::~chess() {
}

void chess::draw(RenderWindow &window) {
    this->pos = Mouse::getPosition(window);
    if (isMove) P[n].setPosition(pos.x - dx, pos.y - dy); //seta a posiçao momentania da peça em hold no click
    window.draw(bBoard);                                                                                    //do mouse
    for (int i = 0; i < 32; ++i) window.draw(P[i]);
}

string chess::setMove(bool bMove) {
    int cont = 31;
    if (bMove) {
        for (int i = 0; i < 32; ++i) {
            if (P[i].getGlobalBounds().contains(pos.x, pos.y)) { //verifica se o click do mouse foi em alguma peça
                isMove = true;
                this->n = i; //guarda o valor do i em n para saber com qual peça esta sendo lidada no vetor de sprite
                dy = pos.y - P[i].getPosition().y; //guarda as posicoes x e y para na funcao draw ocorrer a animaçao
                dx = pos.x - P[i].getPosition().x;  //das peças se mexendo ao movimentar o mouse em click
                oldPos = P[i].getPosition(); // guarda a posiçao final
            }
            cont--;
        }
    } else {
        isMove = false; // entra aqui quando o click do mouse esquedo foi solto
        Vector2f aux_pos = P[n].getPosition() + Vector2f(tam / 2, tam / 2); // faz um calculo das coordenadas
        newPos = Vector2f(tam * int(aux_pos.x / tam), tam * int(aux_pos.y / tam));//e transforma na nova posiçao
        str = notas(oldPos) + notas(newPos); //chama a funca notas para gerar a posiçao em string exe "a2a3"
        move(str); //faz o movimento a partir de string
        if (oldPos != newPos) posicao += str + " ";
        P[n].setPosition(newPos); //seta a posiçao n para a nova posicao correta
        return str; //retorna o movimento para poder ser mandado ao oponete
    }
    return "";
}

void chess::loadPosition() { //load inicial das peças
    int k = 0, next;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            next = board[i][j];
            if (!next) continue;
            int x = abs(next) - 1;
            int y = next > 0 ? 1 : 0; //gera um TextuRect para poder ter controle sobre cada sprite
            P[k].setTextureRect(IntRect(tam * x, tam * y, tam, tam));
            P[k].setPosition(tam * j, tam * i);
            k++;
        }
        for (int j = 0; j < posicao.length(); j += 5) move(posicao.substr(j, 5));
    }
}

String chess::notas(Vector2f pp) { //esta funcao transforma uma coordenada xy e coordenada de tabuleiro ex: "a2"
    string S = "";
    S += char(pp.x / tam + 97);
    S += char(7 - pp.y / tam + 49);
    return S;
}

Vector2f chess::Coord(char a, char b) { //esta funcao faz a conversao de coordenada de tabuleiro para xy
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;

    return Vector2f(x * tam, y * tam);
}

void chess::apaga_pos() { //apaga as posicoes movidas
    if (!posicao.empty()) posicao.erase(posicao.length() - 5, 5);
    loadPosition();
}

void chess::move(string mov) { //a funcao move recebe uma string contendo a posicao antiga da peca e a posicao para qual
    Vector2f auxOld = Coord(mov[0], mov[1]); //voce deseja move a peça
    Vector2f auxNew = Coord(mov[2], mov[3]); //aqui chama a funcao que tranforma a coordenada de string em xy

    for (int i = 0; i < 32; ++i)
        if (P[i].getPosition() == auxOld) P[i].setPosition(auxNew); //compara para saber qual sprite esta na posiçao
                                                                    //que deseja movar e assim seta ela para a nova posiçao
    for (int i = 0; i < 32; ++i)
        if (P[i].getPosition() == auxNew) P[i].setPosition(-100, -100); //faz um tratamento para caso haja uma peça
                                                                            //na nova posiçao essa peça é movida para
    if (mov == "e1g1") if (posicao.find("e1") == -1) move("h1f1");   //(-100,-100), assim ele é considerada excluida
    if (mov == "e8g8") if (posicao.find("e8") == -1) move("h8f8");      //logicamente
    if (mov == "e1c1") if (posicao.find("e1") == -1) move("a1d1");
    if (mov == "e8c8") if (posicao.find("e8") == -1) move("a8d8");
}

void chess::setCoisas(RenderWindow &window, string mov) { //esta funcao é utilizada para fazer o movimento recebido do
    str = mov;                                              //oponete, a string contendo o movimento feito.
    oldPos = Coord(str[0], str[1]); //tranforma para coordenada xy
    newPos = Coord(str[2], str[3]);

    for (int i = 0; i < 32; ++i) if (P[i].getPosition() == oldPos) n = i; //verifica qual sprite é para mover

    for (int i = 0; i < 50; ++i) { //faz a animaçao de movimento
        Vector2f aux = newPos - oldPos;
        P[n].move(aux.x / 50, aux.y / 50);
        window.draw(bBoard);
        for (int j = 0; j < 32; ++j) window.draw(P[j]);
        window.draw(P[n]);
        window.display();
    }
    move(str);
    posicao += str + " ";
    P[n].setPosition(newPos); //seta na posiçao exata no tabuleiro
}

int chess::check() { //faz a verificaçao se os reis de cada um sofreu checkmate ou não
    if (player == 2) {
        if (P[3].getPosition() == Vector2f(-100, -100)) return 2;
        else if (P[27].getPosition() == Vector2f(-100, -100)) return 1;
    }
    if (player == 1) {
        if (P[3].getPosition() == Vector2f(-100, -100)) return 1;
        else if (P[27].getPosition() == Vector2f(-100, -100)) return 2;
    }
    return 0;
}

