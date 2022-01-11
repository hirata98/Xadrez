
Blitz Chess:
    Um jogo de xadrez simples e rápido, voltado para pessoas com um conhecimento prévio sobre as regras de xadrez.
    Desenvolvido utilizando sockets para gerar uma conexão cliente-servidor local.
Compilação:
     O trabalho foi desenvolvido utilizando o Clion, assim, dentro do CMakeList se encontra dois comandos
     de compilação semelhantes para poder compilar o executável do servidor e do cliente.
     No comando add_executable é feito a linkagem de todas as bibliotecas com o servidor.cpp e um outro add_executable
     para o cliente.cpp assim é possível geral duas ferramentas de build no Clion.
     Como se trata de uma conexão cliente-servidor é preciso fazer a compilação inicialmente do servidor para
     ele poder ficar em estado de listen e ao executar o cliente, tenta-se a conexão entre eles.
Jogar:
    Ao executar o jogo o servidor fica em estado de aguardo e o cliente precisa clicar em play para iniciar.
    O cliente por ser as peças brancas, inicia a partida fazendo um movimento e em seguida o movimento é transmitido
    para o servidor. O servidor faz o seu movimento que é mandado para o cliente e assim sucessivamente até alguem dar
    xeque-mate.
