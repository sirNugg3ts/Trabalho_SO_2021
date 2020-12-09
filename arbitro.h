#ifndef ARBITRO
#define ARBITRO

//valores a ser usados caso não sejam recebidos valores ou não estejam definidos
#define MAXPLAYERS 10
#define GAMEDIR "~/gamedir/"
#define DURACAOCAMPEONATO 300000 
#define TEMPOESPERA 60000 

//struct para guardar as variáveis todas
typedef struct VARS{
    int duracaoCampeonato;
    int tempoEspera;
    char *gamedir;
	int maxplayers;
}VARS;

//aquilo que o arbitro precisa de saber do campeonato
typedef struct arbitro{
    int nJogadores;
}infoArbitro;

//pipe zone

//named pipe server -> client
typedef struct pipeSC{
    char fifostr[100]; //nome ficheiro
    int pid; //pid do cliente
    char cmd[30]; //comando a enviar
    char args[50];
}pipeSC;

#endif