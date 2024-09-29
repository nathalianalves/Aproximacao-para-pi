parametrosCompilacao=-Wall -lm
nomePrograma=pi

all: $(nomePrograma)

$(nomePrograma): piApproximation.o 
	gcc -o $(nomePrograma) piApproximation.o $(parametrosCompilacao)

piApproximation.o: piApproximation.c
	gcc -c piApproximation.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch

purge:
	make clean
	rm -f $(nomePrograma)
