#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>

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

// variaveis de contagem de pessoas
int millisIn = 0;
int millisOut = 0;
Long movimento = 0;

// Chamada de funcoes


// Funcao principal
int int main(int argc, char const *argv[]) {
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

    return 0;
}

void ativaPresenca(void) {
    mensagemAlerta(Presenca);
}

void ativaFumaca(void) {
    mensagemAlerta(Fumaca);
}

void ativaJanela(void) {
    mensagemAlerta(Janela);
}

void ativaPorta(void) {
    mensagemAlerta(Porta);
}

void ativaEntrada(void) {
    long millisIn = millis();
    
    if(millisIn - movimento > 200){
        atualizaTotalPessoas(Entrada);
    }
    
    movimento = millisIn;
}

void ativaSaida(void) {
    long millisOut = millis();

    if(millisOut - movimento > 200){
        atualizaTotalPessoas(Saida);
    }
    
    movimento = millisOut;
}

void mensagemAlerta(int sensor){
    switch(sensor) {
      case Presenca :
         printf("Sensor de presenca ativado!\n" );
         break;
      case Fumaca :
         printf("Sensor de fumaca ativado!\n" );
         break;
      case Janela :
         printf("Sensor de janela ativado!\n" );
         break;
      case Porta :
         printf("Sensor de porta ativado!\n" );
         break;
      default :
         printf("Nenhum sensor ativado...\n" );
   }
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