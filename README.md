# FSE - Trabalho 1 - 2022/2
Trabalho 1 de Fundamentos de Sistemas Embarcados

Aluno   | Matrícula
--------- | ------
Victor Amaral Cerqueira | 17/0164411

## Objetivo 
Este trabalho tem por objetivo a criação de um sistema distribuído de automação predial para monitoramento e acionamento de sensores e dispositivos de um prédio com múltiplas salas. O sistema deve ser desenvolvido para funcionar em um conjunto de placas Raspberry Pi com um servidor central responsável pelo controle e interface com o usuário e servidores distribuídos para leitura e acionamento dos dispositivos. Dentre os dispositivos envolvidos estão o monitoramento de temperatura e umidade, sensores de presença, sensores de fumaça, sensores de contagem de pessoas, sensores de abertura e fechamento de portas e janelas, acionamento de lâmpadas, aparelhos de ar-condicionado, alarme e aspersores de água em caso de incêndio.

## Instalação

Para instalação, basta executar:

```bash
cd Central
make
```

Para instalar o servidor Central.

E:

```bash
cd Distribuido
make
```

Para instalar o servidor Distribuído.


## Execução

Para executar, é necessário executar o Servidor Central + Duas instâncias do servidor distribuído na RASP 44, seguindo os comandos:

```bash
cd Central
bin/bin
```

```bash
cd Distribuido
bin/bin config1.txt
```

```bash
cd Distribuido
bin/bin config2.txt
```

Sendo `config1.txt` ou `config2.txt` os arquivos de configuração das portas da RASP.
Assim, executando na RASP 43 ou na RASP 44, os 4 cruzamentos serão executados.

## Modo de Uso

Acionando os sensores e botões através da plataforma online.

## Referências
- [repositório semestres anteriores - Autor: Juniopereira](https://github.com/juniopereirab/FSE-Trabalho1)
- [socket](https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/)
