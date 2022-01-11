#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include "chess.h"
#include "Buttun.h"
#include "win_lose.h"
#include <cstring>

using namespace sf;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

//main do servidor, ele fica em um estado de espera aguardando um cliente conectar
//ocorrendo a conexao inicia a tela em ambos cliente e servidor.
int __cdecl main(void) {
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    bool winb = false, loseb = false, start = false; //verificadores para saber se perdeu ou ganhou
    //verificador para saber se o jogo iniciou ou esta na tela inicial
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(453, 453), "Servidor");

    //classes
    Buttun playbuttun(2);
    win_lose winLose;
    string move, move2; //usados para fazer a leitura do buffer recebido em forma de string

    chess the_game(2);//classe do jogo de xadrez recebe valor 2 referente ao servidor
    the_game.loadPosition();//set as posicoes inicias no tabuleiro
    sendbuf[1] = '\0';

    do {
        Event event;
        if (the_game.check() == 1) {
            winb = true;
            cout << "checkmate, voce ganhou" << endl;
        } else if (the_game.check() == 2) {
            loseb = true;
            cout << "checkmate, voce perdeu" << endl;
        }
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (recvbuf[0] == 'x') start = true;
        if (iResult == 4) {
            move = recvbuf;
            the_game.setCoisas(window, move);
            sendbuf[1] = '\0';
            iResult = 1;
        }
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::BackSpace) the_game.apaga_pos();
                if (event.key.code == Keyboard::Escape) window.close();
            }
            //o primeiro if chama a funçao da classe chess para setar o movimento feito na tela do servidor
            if (event.type == Event::MouseButtonPressed) if (event.key.code == Mouse::Left) the_game.setMove(true);
            if (event.type == Event::MouseButtonReleased) { //ao soltar o clique do mouse chama a funçao porem para
                if (event.key.code == Mouse::Left) {        //setar a posiçao e retornar o movimento feito em string
                    move2 = the_game.setMove(false);        //para poder mandar para o cliente.
                    for (int i = 0; i < size(move2); ++i) sendbuf[i] = move2[i];
                    sendbuf[size(sendbuf) - 1] = '\0';
                }
            }
            // Close window: exit
            if (event.type == Event::Closed) window.close();
        }
        send(ClientSocket, sendbuf, (int) strlen(sendbuf), 0); //manda uma string contendo o movimento ou nada
        sendbuf[1] = '\0';
        // Clear screen
        window.clear();
        // Draw the sprite
        if (!start) playbuttun.draw(window); //tela inicial antes do jogo iniciar, chamada
        if (!winb && !loseb && start) the_game.draw(window); // a partir do momento que o cliente clicou em play
        else {
            if (winb) winLose.draw(window, 1);//chama para desenhar as imagens de perdeu ou ganhou
            if (loseb) winLose.draw(window, 2);
        }
        // Update the window
        window.display();
    } while (iResult > 0 && window.isOpen());

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}