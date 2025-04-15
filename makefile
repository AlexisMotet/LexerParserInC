start_visiteur : main.o visiteur_dummy.o visiteur.o parser.o lexer.o
	gcc -o start_visiteur main.o visiteur_dummy.o visiteur.o parser.o lexer.o
main.o : main.c 
	gcc -c main.c
visiteur_dummy.o : visiteur_dummy.c
	gcc -c visiteur_dummy.c
visiteur.o : visiteur.c
	gcc -c visiteur.c
parser.o : parser.c
	gcc -c parser.c
lexer.o : lexer.c
	gcc -c lexer.c
clean : 
	rm -f *.o