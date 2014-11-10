Hwatoo.exe: Debug_Center.o HMain.o HDeck.o HCard.o HPlayer.o HGame.o
	gcc -std=c99 -D DEBUG -g -o Hwatoo.exe Debug_Center.o HMain.o HDeck.o HCard.o HPlayer.o HGame.o

Debug_Center.o: Debug_Center.c
	gcc -std=c99 -D DEBUG -g -c Debug_Center.c

HMain.o: HMain.c
	gcc -std=c99 -D DEBUG -g -c HMain.c

HDeck.o: HDeck.c
	gcc -std=c99 -D DEBUG -g -c HDeck.c

HCard.o: HCard.c
	gcc -std=c99 -D DEBUG -g -c HCard.c

HPlayer.o: HPlayer.c
	gcc -std=c99 -D DEBUG -g -c HPlayer.c

HGame.o: HGame.c
	gcc -std=c99 -D DEBUG -g -c HGame.c
