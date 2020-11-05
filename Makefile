all: cliente arbitro jogo

cliente: 
	gcc cliente.c cliente.h -o cliente
arbitro:
	gcc arbitro.c arbitro.h -o arbitro
jogo:
	gcc jogo.c jogo.h -o jogo
clean:
	rm  cliente jogo arbitro
