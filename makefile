todo: main.o editor.o
	g++ -Wall -o main main.o editor.o
main.o: main.c
	g++ -Wall -c main.c
editor.o: editor.h editor.c
	g++ -Wall -c editor.c
# se deben agregar aca las reglas de compilacion para los modulos que agreguen
# agregar tambien las dependencias en la regla todo:
clean:
	rm -f main
	rm -f *.o

