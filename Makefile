MORE=../morec


pman: pman.o commandHandler.o more.utf8.o more.fields.o more.common.o
	gcc -o pman pman.o commandHandler.o more.utf8.o more.fields.o -lpthread

pman.o: pman.c
	gcc -c pman.c

commandHandler.o: commandHandler.c
	gcc -c -I$(MORE) commandHandler.c

more.common.o: $(MORE)/more/common.c
	gcc -c -o more.common.o $(MORE)/more/common.c

more.utf8.o: $(MORE)/more/utf8.c
	gcc -c -o more.utf8.o $(MORE)/more/utf8.c

more.fields.o: $(MORE)/more/fields.c
	gcc -c -o more.fields.o $(MORE)/more/fields.c

clean:
	rm -f pman *.o *~
