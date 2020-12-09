all: cliente arbitro jogo

cliente: cliente.c
	gcc cliente.c  -o cliente.o
arbitro: arbitro.c
	gcc arbitro.c  -o arbitro.o
jogo: jogo.c
	gcc  jogo.c -o jogo.o

clean:
	rm -f *.o