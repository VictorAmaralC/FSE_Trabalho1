#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>

#include "socket.c"

#define false 0
#define true 1

// Definicao Estrutura de sala
struct Sala {
    int pessoa;
    int total;
};

Sala contadorSala;

// Variaveis de definicao de sensores
int Lampada1;
int Lampada2;
int ArCondicionado;
int Projetor;
int Alarme;
int Presenca;
int Fumaca;
int Janela;
int Porta;
int Entrada;
int Saida;
int temperatura;

// variaveis gerais
int tempoEntrada = 0;
int tempoSaida = 0;
long ultimoMovimento = 0;
long tempoPresenca = 0;
char *aviso;
bool alarmeTocando = false;
bool luzesGerais = false;

// Definições para Thread's e Socket
pthread_t thread1;
pthread_t thread2;
int socket, connfd;

// Chamada de funcoes
void configuraPino();
void ativaPresenca(void);
void ativaFumaca(void);
void ativaJanela(void);
void ativaPorta(void);
void ativaEntrada(void);
void ativaSaida(void);
void mensagemAlerta(void);
void atualizaTotalPessoas(void);

// Funcao principal
int int main(int argc, char const *argv[]) {
    
    pthread_create(&thread1, NULL, thread_func, NULL);

    if(wiringPiSetup() == -1) return 1;

    FILE* arquivoSala;

    arquivoSala = fopen(argv[1], "r");
    
    if(arquivoSala) {
        fscanf(arquivoSala, "%d", &Lampada1);
        fscanf(arquivoSala, "%d", &Lampada2);
        fscanf(arquivoSala, "%d", &ArCondicionado);
        fscanf(arquivoSala, "%d", &Projetor);
        fscanf(arquivoSala, "%d", &Alarme);
        fscanf(arquivoSala, "%d", &Presenca);
        fscanf(arquivoSala, "%d", &Fumaca);
        fscanf(arquivoSala, "%d", &Janela);
        fscanf(arquivoSala, "%d", &Porta);
        fscanf(arquivoSala, "%d", &Entrada);
        fscanf(arquivoSala, "%d", &Saida);
        fscanf(arquivoSala, "%d", &temperatura);
    } else {
        printf("Arquivo de configuração de sala não encontrado/invalido\n");
    }

    fclose(arquivoSala);

    configuraPino();
    
    //Loop checagem de presença para desligar luzes gerais
    while(1){
        long checagem = millis();
        
        if ((checagem - tempoPresenca >= 1500) && luzesGerais == true){
            digitalWrite(Lampada1, 0);
            digitalWrite(Lampada2, 0);
            luzesGerais = false;
        }
        tempoPresenca = checagem;
        
        delay(1000);
    }

    return 0;
}

void configuraPino(){
    pinMode(Lampada1,       OUTPUT);
    pinMode(Lampada2,       OUTPUT);
    pinMode(ArCondicionado, OUTPUT);
    pinMode(Projetor,       OUTPUT);
    pinMode(Alarme,         OUTPUT);
    pinMode(Presenca,       INPUT);
    pinMode(Fumaca,         INPUT);
    pinMode(Porta,          INPUT);
    pinMode(Janela,         INPUT);
    pinMode(Entrada,        INPUT);
    pinMode(Saida,          INPUT);

    pullUpDnControl(Lampada1,       PUD_UP);
    pullUpDnControl(Lampada2,       PUD_UP);
    pullUpDnControl(ArCondicionado, PUD_UP);
    pullUpDnControl(Alarme,         PUD_UP);
    pullUpDnControl(Presenca,       PUD_UP);
    pullUpDnControl(Fumaca,         PUD_UP);
    pullUpDnControl(Porta,          PUD_UP);
    pullUpDnControl(Janela,         PUD_UP);
    pullUpDnControl(Entrada,        PUD_UP);
    pullUpDnControl(Saida,          PUD_UP);

    wiringPiISR(Presenca, INT_EDGE_RISING, &ativaPresenca);
    wiringPiISR(Fumaca,   INT_EDGE_RISING, &ativaFumaca);
    wiringPiISR(Porta,    INT_EDGE_RISING, &ativaPorta);
    wiringPiISR(Janela,   INT_EDGE_RISING, &ativaJanela);
    wiringPiISR(Entrada,  INT_EDGE_RISING, &ativaEntrada);
    wiringPiISR(Saida,    INT_EDGE_RISING, &ativaSaida);

}

void ativaPresenca(void) {
    aviso = "Sensor de presenca ativado!\n";

    if(alarmeTocando == true){
        if(digitalRead(Alarme) == 0)
            digitalWrite(Alarme, 1);
        
        aviso += " Alarme ativado!";
    } else if (alarmeTocando == false){
        digitalWrite(Lampada1, 1);
        digitalWrite(Lampada2, 1);
        
        tempoPresenca = millis();
        luzesGerais = true;
        aviso += " Luzes acesas!";
    }

    enviaAvisoCentral(aviso);
}

void ativaFumaca(void) {
    if(digitalRead(Fumaca) == false){
        aviso = "Sensor de fumaca ativado!\n";
        digitalWrite(Fumaca, 1);
            
        alarmeTocando = true;
        if(digitalRead(Alarme) == 0)
            digitalWrite(Alarme, 1);
    } else {
        aviso = "Sensor de fumaca desativado!\n";
        digitalWrite(Fumaca, 1);
    }
    enviaAvisoCentral(aviso);
}

void ativaJanela(void) {
    if(digitalRead(Janela) == false){
        digitalWrite(Janela, 1);
        aviso = "Sensor de janela ativado!\n";
    } else {
        digitalWrite(Janela, 0);
        aviso = "Sensor de janela desativado!\n";
    } 
    enviaAvisoCentral(aviso);
}

void ativaPorta(void) {
    if(digitalRead(Porta) == false){
        digitalWrite(Porta, 1);
        aviso = "Sensor de porta ativado!\n";
    } else {
        digitalWrite(Porta, 0);
        aviso = "Sensor de porta desativado!\n";
    }

    enviaAvisoCentral(aviso);
}

void ativaEntrada(void) {
    long tempoEntrada = millis();
    
    if(tempoEntrada - ultimoMovimento > 200){
        atualizaTotalPessoas(Entrada);
    }
    
    ultimoMovimento = tempoEntrada;
}

void ativaSaida(void) {
    long tempoSaida = millis();

    if(tempoSaida - ultimoMovimento > 200){
        atualizaTotalPessoas(Saida);
    }
    
    ultimoMovimento = tempoSaida;
}

void enviaAvisoCentral(char *aviso){
    write(socket, aviso, sizeof(aviso));
    bzero(aviso, sizeof(aviso));
}

void atualizaTotalPessoas(int alteracaoMovimento){
    if(alteracaoMovimento == Entrada){
        contadorSala.pessoa++;
        contadorSala.total++;
    } else if(alteracaoMovimento == Saida){
        rcontadorSala.pessoa--;
        contadorSala.total--;
    }
}

void *thread_func (void *arg) {
    struct sockaddr_in endServidor;
   
    socket = setupSocket();
    bzero(&endServidor, sizeof(endServidor));
   
    endServidor.sin_family = AF_INET;
    endServidor.sin_addr.s_addr = inet_addr("164.41.98.16");
    endServidor.sin_port = htons(13508);
   
    connectSocketServer(socket, endServidor);
   
    //pthread_create(&threadB, NULL, thread_funcB, (void*)socket);
    func(socket);   
    close(socket);
}

/*void *thread_funcB (void *arg) {
    char buff[MAX];
    int n;
    int socket = (int) arg;

    while(1) {
        read(socket, buff, MAX);
        printf("%s\n", buff);
    }
}*/

void func(int socket) {
    char buffer[MAX];
    int n;  

    while(1) {
        bzero(buffer, sizeof(buffer));
        buff[0] = contadorSala[n].pessoa;
        buff[1] = contadorSala[n].Total;
        write(socket, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
        delay(2000); //tempo de delay baseado no sensor de temperatura  
    }
}