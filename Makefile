all: cliente arbitro jogo

cliente: cliente.c cliente.h
	gcc cliente.c cliente.h -o cliente.o
arbitro:arbitro.c arbitro.h
	gcc arbitro.c arbitro.h -o arbitro.o
jogo:jogo.c jogo.h
	gcc  jogo.c jogo.h -o jogo.o

clean:
	rm *.o