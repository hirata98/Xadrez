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

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

//main do cliente ele solicita uma conexao com servidor, ocorrendo a conexao as telas de ambas se iniciam
int __cdecl main(int argc, char **argv) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int) ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Servidor offline!\n");
        WSACleanup();
        return 1;
    }

    bool winb = false, loseb = false; //verificadores para saber se perdeu ou ganhou
    int start = 0; //verificador para saber se o jogo iniciou ou esta na tela inicial
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(453, 453), "Cliente");

    //classes
    Buttun playbuttun(1); // esta classe contem um botao play uilizado somente no cliente pois é ele que inicia o jogo
    win_lose winLose;
    string move, move2; //usados para fazer a leitura do buffer recebido em forma de string

    chess the_game(1); //classe do jogo de xadrez recebe valor 1 referente ao cliente
    the_game.loadPosition(); //set as posicoes inicias no tabuleiro

    sendbuf[1] = '\0';
    // Start the game loop
    // Process events
    do {
        Event event;
        if (the_game.check() == 1) {
            cout << "checkmate, voce ganhou" << endl;
            winb = true;
        } else if (the_game.check() == 2) {
            cout << "checkmate, voce perdeu" << endl;
            loseb = true;
        }
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::BackSpace) the_game.apaga_pos();
                if (event.key.code == Keyboard::Escape) window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.key.code == Mouse::Left) {
                    if (start == 0) {
                        if (playbuttun.isMouseOver(window)) start = 1;
                    } else if (start == 2) the_game.setMove(true);
                }
            }
            if (event.type == Event::MouseButtonReleased) {
                if (event.key.code == Mouse::Left) {
                    move2 = the_game.setMove(false);
                    for (int i = 0; i < size(move2); ++i) sendbuf[i] = move2[i];
                    sendbuf[size(sendbuf) - 1] = '\0';
                }
            }
            // Close window: exit
            if (event.type == Event::Closed) window.close();
        }
        if (start == 1) {
            start = 2;
            send(ConnectSocket, "x", 1, 0);
        } else send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
        sendbuf[1] = '\0';
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult == 4) {
            move = recvbuf;
            the_game.setCoisas(window, move);
            sendbuf[1] = '\0';
        }
        sendbuf[1] = '\0';
        // Clear screen
        window.clear();
        // Draw the sprite
        if (start == 0) playbuttun.draw(window); //tela inicial antes do jogo iniciar chamada
        if (!winb && !loseb && start == 2) the_game.draw(window); // a partir do momento que o cliente clicou em play
        else {                                                       //chamada
            if (winb) winLose.draw(window, 1); //chama para desenhar as imagens de perdeu ou ganhou
            if (loseb) winLose.draw(window, 2);
        }
        // Update the window
        window.display();

    } while (iResult > 0 && window.isOpen());
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}