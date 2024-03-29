#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "arbitro.h"
#include "utils.h"
#include "cliente.h"
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <dirent.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <time.h>

int abre_serverPipe();
int abre_clientPipe(pid_t pidCliente);
int verificaSeJogadorExiste(CLIENT *jogador, CLIENT *listaJogadores, int nJogadoresRecebidos);
void atribuiJogo(CLIENT* jogador,pJogo listaJogos);

int main(int argc, char **argv)
{
	
	setbuf(stdout,NULL);
	setbuf(stdin,NULL);


	VARS variaveis;
	infoArbitro arbitroSettings;

	variaveis.duracaoCampeonato = -1;
	variaveis.tempoEspera = -1;
	arbitroSettings.nJogadores = 0;

	//Obtenção das variáveis de ambiente
	if (getenv("GAMEDIR") != NULL)
		variaveis.gamedir = getenv("GAMEDIR");
	else
	{
		printf("\nVariavel de ambiente GAMEDIR nao detetada, será usado o diretorio atual");
		variaveis.gamedir = ".";
	}

	if (getenv("MAXPLAYERS") != NULL)
	{
		if (atoi(getenv("MAXPLAYERS")) > 30)
		{
			printf("\nValor invalido definido na variavel de ambiente MAXPLAYERS, voltando para default -> %d", MAXPLAYERS);
			variaveis.maxplayers = MAXPLAYERS;
		}
		else
			variaveis.maxplayers = atoi(getenv("MAXPLAYERS"));
	}
	else
	{
		printf("\nVariavel de ambiente MAXPLAYERS nao definida, voltando para default -> %d", MAXPLAYERS);
		variaveis.maxplayers = MAXPLAYERS;
	}

	//obter lista de jogos
	pJogo listaJogos = NULL;

	DIR *d;
	struct dirent *dir;
	d = opendir(variaveis.gamedir);

	if(d == NULL){
		printf("\nA pasta ~/gamedir nao existe, o diretorio atual sera usado");
		d = opendir(".");

		if (d == NULL)
		{	
			printf("\nAlgo de muito errado acabou de acontecer, a sair");
			exit(EXIT_FAILURE);
		}
	}

	while ((dir= readdir(d)) != NULL)
	{
			if (strstr(dir->d_name,"g_") != NULL) //verifica se é um jogo
			{
				pJogo newGame = malloc(sizeof(jogo));
				strcpy(newGame->gamename,dir->d_name);
				newGame->next = NULL;

				if (listaJogos == NULL)
					listaJogos = newGame;

				else{
					pJogo aux = listaJogos;
					while (aux->next != NULL)
						aux = aux->next;
					
					aux->next = newGame;
					
				}
			}
	}

	if(listaJogos==NULL){
		printf("\nNenhum jogo foi detetado, o campeonato nao pode continuar\n");
		exit(EXIT_FAILURE);
	}

	printf("\nLista de jogos detetados:");
	pJogo aux = listaJogos;
	for (int i = 0;aux!=NULL; i++)
	{
		printf("\n%s",aux->gamename);
		aux = aux->next;
	}
	//obtenção da duração do campeonato e tempo de espera

	int indice;

	while ((indice = getopt(argc, argv, "c:t:")) != -1) //usage: ./arbitro -c 60000 -t 10000
	{
		switch (indice)
		{
		case 'c':
			variaveis.duracaoCampeonato = atoi(optarg);
			printf("\nDuracao campeonato recebida: %d", variaveis.duracaoCampeonato);
			break;

		case 't':
			variaveis.tempoEspera = atoi(optarg);
			printf("\nTempo de espera recebido: %d", variaveis.tempoEspera);
			break;
		default:
			printf("\nArgumento %c inválido", indice);
			break;
		}
	}

	//caso não receba o valor na linha de comandos fica com estes valores
	if (variaveis.tempoEspera == -1)
	{
		printf("\nNao foi recebido nenhum valor para tempo de espera, defaulting to %d", TEMPOESPERA);
		variaveis.tempoEspera = TEMPOESPERA;
	}
	if (variaveis.duracaoCampeonato == -1)
	{
		printf("\nNao foi recebido nenhum valor para a duracao do campeonato, defaulting to %d", DURACAOCAMPEONATO);
		variaveis.duracaoCampeonato = DURACAOCAMPEONATO;
	}

	printf("\ngamedir:%s\nmaxplayers:%d\ntempo espera:%d\nduracao campeonato:%d", variaveis.gamedir, variaveis.maxplayers, variaveis.tempoEspera, variaveis.duracaoCampeonato);


	printf("\n**********\nAberta sessao de registo de jogadores\n\n");
	//1ªfase - obter os jogadores (meta 2)

	struct timeval timeout;
	timeout.tv_sec =variaveis.tempoEspera;

	pCLIENT participantes = NULL; //inicio da lista de participantes
	

	int serverpipe_fd = abre_serverPipe();
	int clientpipe_fd;

	resposta_t resposta;
	pedido_t pedido;

	int nbytes_lidos;
	int nbytes_escritos;

	// select vars
	fd_set fdset, fdset_backup;
	FD_ZERO(&fdset);

	FD_SET(0, &fdset); 
	FD_SET(serverpipe_fd, &fdset);

	int timedout = 1;

	while (timedout)
	{
		
		fdset_backup = fdset;

		switch (select(serverpipe_fd+1, &fdset_backup, NULL, NULL, &timeout))
		{
		case -1:
			if (errno == EINTR)
				continue;
			else{
				unlink(ARBITRO_FIFO);
				exit(EXIT_FAILURE);
			}
			break;

		case 0: //TIMEOUT
			printf("\ntimeout");
			timedout = 0;
			break;

		default:
			if (FD_ISSET(0, &fdset_backup)) //ler STDIN
			{
				int bytescomando;
				char comando[TAM_MAX];

				/* recebe comando do server */
				bytescomando = read(0,comando,sizeof(comando));
				comando[bytescomando] = '\0';

				if (strcmp(comando,"\n")==0)
				{
					
				}else{
					//processar comando
					if(strcmp(comando,"players\n")==0){

						pCLIENT percorre = participantes;
						if (percorre == NULL)
						{
							printf("\nAinda nao ha jogadores registados no campeonato\n");
						}
						
						for (int i = 0; i < arbitroSettings.nJogadores; i++)
						{
							printf("\nJogador %d: Nome %s, jogo atribuido %s ,PID %d\n",i+1,percorre->jogador.nome,percorre->jogador.jogo,percorre->pidsender);
							percorre = percorre->nextClient;
						}
						
					}else if(strcmp(comando,"games\n")==0){
						pJogo aux = listaJogos;
						while(aux != NULL){
							printf("\n%s",aux->gamename);
							aux = aux->next;
						}
						
					}else if(comando[0]=='k'){
						char playertokill[TAM_MAX];
						for (int i = 1;comando[i] != '\0' && comando[i] != '\n'; i++)
						{
							playertokill[i-1] = comando[i];
						}
						
						printf("\nIm going to kill player %s ",playertokill);

						pCLIENT percorre = participantes;
						pCLIENT before = NULL;

						while (strcmp(percorre->jogador.nome,playertokill)!=0)
						{
							before = percorre;
							percorre = percorre->nextClient;
						}
						
						if (before == NULL)
						{

							participantes = participantes->nextClient;
							free(percorre);
							arbitroSettings.nJogadores--;
						}else{
							before->nextClient = percorre->nextClient;
							free(percorre);
							arbitroSettings.nJogadores--;
						}

						//falta enviar um sinal que representa que desistiu? ainda nao sabemos
						//o que fazer nesse caso
						
						

					}else if(strcmp(comando,"exit\n")==0){
						unlink(ARBITRO_FIFO);
						exit(EXIT_SUCCESS);
					}
				}
			}

			if (FD_ISSET(serverpipe_fd, &fdset_backup))
			{
				CLIENT *newClient = malloc(sizeof(CLIENT));
				nbytes_lidos = read(serverpipe_fd, &pedido, sizeof(pedido_t));
				if (nbytes_lidos == -1)
				{
					printf("\nErro ao ler o pipe\n");
				}
				printf("\n[CLIENTE %d]jogador: %s", pedido.pidsender, pedido.nomeJogador);
				newClient->pidsender = pedido.pidsender;
				strcpy(newClient->jogador.nome,pedido.nomeJogador);
				newClient->nextClient = NULL;
				printf("\nNome do novo cliente: %s",newClient->jogador.nome);
				newClient->jogador.pontuacao = 0;

				if (arbitroSettings.nJogadores == variaveis.maxplayers)
				{
					strcpy(resposta.resposta, "Servidor cheio!");
					printf("\n[SERVER] responding with -> %s", resposta.resposta);
					clientpipe_fd = abre_clientPipe(pedido.pidsender);
					nbytes_escritos = write(clientpipe_fd, &resposta, sizeof(resposta));
					if (nbytes_escritos == -1)
					{
						printf("\nErro ao escrever no pipe\n");
					}

					close(clientpipe_fd);
					free(newClient);
				}
				printf("\nvou verificar se ja existe");
				if (verificaSeJogadorExiste(newClient,participantes, arbitroSettings.nJogadores) == 0)
				{
					pCLIENT aux = participantes;


					atribuiJogo(newClient,listaJogos);

					if (aux == NULL)
					{
						participantes = newClient;
					}else
					{
						while (aux->nextClient!=NULL)
					{
						aux = aux->nextClient;
					}

					aux->nextClient = newClient;
					}
					sprintf(resposta.resposta,"Jogador aceite com sucesso! Foi lhe atribuido o jogo %s\n",newClient->jogador.jogo);
					arbitroSettings.nJogadores++;
					strcpy(resposta.jogoAtribuido,newClient->jogador.jogo);
					printf("\n[SERVER] responding with -> %s", resposta.resposta);
					fflush(stdout);
					clientpipe_fd = abre_clientPipe(pedido.pidsender);
					nbytes_escritos = write(clientpipe_fd, &resposta, sizeof(resposta));
					if (nbytes_escritos == -1)
					{
						printf("\nErro ao escrever no pipe");
					}
					close(clientpipe_fd);
				}
				else
				{
					strcpy(resposta.resposta, "Repetido!");
					printf("\n[SERVER] responding with -> %s\n", resposta.resposta);
					clientpipe_fd = abre_clientPipe(pedido.pidsender);
					nbytes_escritos = write(clientpipe_fd, &resposta, sizeof(resposta));
					if (nbytes_escritos == -1)
					{
						printf("\nErro ao escrever no pipe");
					}
					close(clientpipe_fd);
					free(newClient);
				}
			}
			break;
		}
	}
	unlink(ARBITRO_FIFO);
	return EXIT_SUCCESS;
}

int abre_serverPipe()
{
	int serverpipe_fd;

	if (access(ARBITRO_FIFO, F_OK) == -1)
	{
		if (mkfifo(ARBITRO_FIFO, 0666) != 0)
			//nao foi possivel abrir o pipe
			exit(EXIT_FAILURE);

		if ((serverpipe_fd = open(ARBITRO_FIFO, O_RDWR)) == -1)
			exit(EXIT_FAILURE);
	}
	else
	{
		printf("\nFile exists, se eu sair e porque deu erro ao abrir");
		if ((serverpipe_fd = open(ARBITRO_FIFO, O_RDWR)) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}

	return serverpipe_fd;
}


int abre_clientPipe(pid_t pidCliente)
{
	int clientpipe_fd;
	char pipe[100];

	sprintf(pipe, CLIENT_FIFO, pidCliente);

	clientpipe_fd = open(pipe, O_WRONLY);
	if (clientpipe_fd == -1)
	{
		printf("\n nao consegui abrir o pipe do cliente");
		exit(EXIT_FAILURE);
	}
	return clientpipe_fd;
}

int verificaSeJogadorExiste(CLIENT *jogador, CLIENT *listaJogadores, int nJogadoresRecebidos)
{
	pCLIENT aux = listaJogadores;

	for (int i = 0; i < nJogadoresRecebidos; i++)
	{
		if (strcmp(aux->jogador.nome, jogador->jogador.nome) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void atribuiJogo(CLIENT* jogador,pJogo listaJogos){
	int njogos = 0;
	pJogo aux = listaJogos;
	while (aux!=NULL)
	{
		njogos++;
		aux = aux->next;
	}
	
	srand(time(NULL));
	int nEscolhido = rand() % njogos +1;

	aux = listaJogos;
	for (int i = 1; i != nEscolhido; i++)
	{
		aux=aux->next;
	}
	strcpy(jogador->jogador.jogo,aux->gamename);
	printf("\nJogo atribuido -> %s\n",jogador->jogador.jogo);
	
}