Hwatoo.exe: HMain.o HDeck.o HCard.o
	gcc -std=c99 -g -o Hwatoo.exe HMain.o HDeck.o HCard.o 

HMain.o: HMain.c
	gcc -std=c99 -g -c HMain.c

HDeck.o: HDeck.c
	gcc -std=c99 -g -c HDeck.c

HCard.o: HCard.c
	gcc -std=c99 -g -c HCard.c
