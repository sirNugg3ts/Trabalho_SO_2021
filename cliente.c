#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"
#include "utils.h"

int main(int argc, char **argv)
{

	//obter identificação do jogador
	PLAYER jogador;
	printf("\nInsira o seu nome:");
	scanf("%s", jogador.nome);
	printf("\nNome recebido:%s", jogador.nome);

	int s_fifo_fd; //fifo servidor
	int c_fifo_fd; //fifo deste cliente
	pedido_t pedido;
	resposta_t resp;

	char c_fifo_name[25]; //nome do fifo deste cliente
	int read_res;

	//criar fifo do cliente
	pedido.pid_cliente = getpid();
	sprintf(c_fifo_name, CLIENT_FIFO, pedido.pid_cliente);

	if (mkfifo(c_fifo_name, 0777) == -1)
	{
		perror("\nmkfifo FIFO cliente deu erro");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "\nFIFO do cliente criado");

	//abre o FIFO do servidor p/ escrever

	s_fifo_fd = open(ARBITRO_FIFO, O_WRONLY);

	if (s_fifo_fd == -1)
	{
		fprintf(stderr, "\nO arbitro nao esta a correr");
		unlink(c_fifo_name);
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "\nFIFO do servidor aberto para WRITE / BLOCKING");

	c_fifo_fd = open(c_fifo_name, O_RDWR);

	if (c_fifo_fd == -1)
	{
		perror("\nErro ao abrir FIFO do cliente");
		close(s_fifo_fd);
		unlink(c_fifo_name);
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "\nFIFO do cliente aberto para READ (+WRITE) BLOCK");

	memset(pedido.comando, '\0', TAM_MAX);

	while (1)
	{
		//obtem comando
		printf("\nComando: ");
		scanf("%s", pedido.comando);

		//evnia o comando para o servidor
		write(s_fifo_fd, &pedido, sizeof(pedido));

		//recebe a resposta
		read_res = read(c_fifo_fd, &resp, sizeof(resp));
		if (read_res == sizeof(resp))
		{
			printf("%s", resp.resposta);
		}
		else
		{
			printf("\nSem resposta ou resposta incompreensivel"
				   "[Bytes lidos: %d]",
				   read_res);
		}
	}

	close(c_fifo_fd);
	close(s_fifo_fd);
	unlink(c_fifo_name);
	return 0;

	return EXIT_SUCCESS;
}
