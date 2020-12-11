CC=gcc -Wall

all: cliente arbitro jogo

cliente: cliente.o
	$(CC) $< -o $@

cliente.o: cliente.c cliente.h utils.h
	$(CC) -c cliente.c

arbitro: arbitro.o
	$(CC) $< -o $@

arbitro.o: arbitro.c arbitro.h utils.h cliente.h
	$(CC) -c arbitro.c

jogo: jogo.o
	$(CC) $< -o $@

jogo.o: jogo.c jogo.h
	$(CC) -c jogo.c

clean:
	rm *.o cliente jogo arbitro -f