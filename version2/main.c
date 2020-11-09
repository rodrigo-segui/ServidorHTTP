#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/stat.h>
#include <pthread.h>   // for threading, link with lpthread
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "server_function.h"

//sem_t mutex; // para controlar o contador de threads

int main()
{
    sem_init(&mutex, 0, 1); // Inıcializa mutex com 1.
    printf("----- Servidor HTTP ------\n\n");
    //sem_init(&mutex, 0, 1); // Inıcializa mutex com 1.
    // Variaveis socket servidor, socket cliente
    int socket_server, socket_client, *new_socket_client;
    int opt=1;

    // Declaraçao de porta padrao, taxa maxima, tamanho cliente
    int PORT = 8080;
    int MAX_RATE = 1000;
    int LENGTH_CLIENT;
    int LENGTH_SERVER;

    struct sockaddr_in server, client;
    LENGTH_CLIENT = sizeof(client);
    LENGTH_CLIENT = sizeof(server);

    // Informaçoes do servidor
    printf("Mensagem do Servidor | Status      : Online!\n");
    printf("Mensagem do Servidor | Taxa Maxima : %d \n", MAX_RATE);
    printf("Mensagem do Servidor | Porta       : %d \n\n\n\n", PORT);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; // localhost

    //-----------------SOCKET()--------------------------//
    // CRIAANDO SOCKETE SERVER
    //socket(int domain, int type, int protocol)
    // PARAMETROS
    // 1 - AF_INET protocolo para comunicaçao ipv4
    // 2 - tipo de comunicacao - sock_stream - comunicacao de dois lados baseada em conexao
    // 3 - protocolo tcp
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    // verificacao de erro socket
    if (socket_server < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    //----------------------------------------------------//

    

    //---------------------BIND()--------------------------//
    // Associa o socket a um endereco na rede/local - funcao bind
    // bind(int sockfd, const struct sockaddr *addr, soclen_t addrlen)
    // ARGUMENTOS
    // 1 - sockfd -socket que queremos associar um endereco a ele
    // 2 - ponteiro para uma estrutura socketaddr- info familia de protocolos, endereco na rede local  e porta
    // 3 -tamanho em bytes das estruturas passadas no segundo argumento

    if (bind(socket_server, (struct sockaddr *)&server, sizeof(server)) < 0) { // vincula socket ao end e num porta especificados acima
      puts("Binding failed");
      return 1;
    }
    //------------------------------------------------------//


    //-----------------LISTEN()-----------------------------//
    //permitir que o socket receba conexoes
    //maximo de conexoes simulaneas
    if (listen(socket_server, MAX_CONNECTIONS) == -1)
    {
       perror("ERRO listen");
       exit(EXIT_FAILURE);
    }
    //-------------------------------------------------------//
    int c;
    int n_conexao = 1;
    c = sizeof(struct sockaddr_in);


    
    int escolha = 0;
    printf("1 - HTTP 1.0 - Conexoes nao-persistentes\n");
    printf("2 - HTTP 1.1 - Conexoes persistentes\n");
    printf("Escolha: ");
    scanf("%d", &escolha);
    printf("\n\n");
    if(escolha == 1){
        printf("---------------------------------------\n");
        printf("HTTP 1.0 - Conexoes nao-persistentes\n");
        printf("---------------------------------------\n\n");

    while ((socket_client = accept(socket_server, (struct sockaddr *)&client, (socklen_t *)&c))){
    
        printf("----Aguardando Conexoes----- ... \n\n");
        //socket_client = accept(socket_server, (struct sockaddr *)&client, (socklen_t *)&c);
        
        printf("CONEXAO:  %i", n_conexao);
        n_conexao ++;

        pthread_t sniffer_thread; // nova thread
        new_socket_client = (int*) malloc(1);
        *new_socket_client = socket_client;
        printf("-----------------");
        printf("%lu", client.sin_addr.s_addr);
        printf("-----------------");

        

        if (pthread_create(&sniffer_thread, NULL, readRequest, (void *)new_socket_client) < 0){ // cria uma thread para cada requisicao, passando socket novo
        puts("Could not create thread");
        return 1;
      }

    }

    close(socket_server);



    }else{
        printf("AINDA NAO IMPLEMENTADO\n");
    }
    
    return 0;


}