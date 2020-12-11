#ifndef UTILS
#define UTILS

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#include "cliente.h"

#define ARBITRO_FIFO "/tmp/server_fifo"

#define CLIENT_FIFO "/tmp/resp_%d_fifo"

#define TAM_MAX 50


typedef struct
{
    pid_t pidsender;
    PLAYER jogador;
    char comando[TAM_MAX];
} pedido_t;

typedef struct
{
    char resposta[TAM_MAX];
} resposta_t;

#endif