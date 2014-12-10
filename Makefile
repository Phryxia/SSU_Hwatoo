Hwatoo.exe: Debug_Center.o HDeck.o HCard.o HPlayer.o HGame.o HGUI.o Renderer.o HNotice.o HFileIO.o HFlow.o HMain.o
	gcc -std=c99 -D DEBUG -g -o Hwatoo.exe Debug_Center.o HCard.o HDeck.o HPlayer.o HGame.o HGUI.o Renderer.o HNotice.o HFileIO.o HFlow.o HMain.o

Debug_Center.o: Debug_Center.c
	gcc -std=c99 -D DEBUG -g -c Debug_Center.c

HMain.o: HMain.c HFlow.o HFileIO.o
	gcc -std=c99 -D DEBUG -g -c HMain.c

HDeck.o: HDeck.c HDeck.h HCard.o
	gcc -std=c99 -D DEBUG -g -c HDeck.c

HCard.o: HCard.c HCard.h
	gcc -std=c99 -D DEBUG -g -c HCard.c

HPlayer.o: HPlayer.c HGame.h HDeck.o
	gcc -std=c99 -D DEBUG -g -c HPlayer.c

HGame.o: HGame.c HGame.h HPlayer.o
	gcc -std=c99 -D DEBUG -g -c HGame.c

HGUI.o: HGUI.c HGUI.h
	gcc -std=c99 -D DEBUG -g -c HGUI.c

Renderer.o: Renderer.c Renderer.h HGUI.o
	gcc -std=c99 -D DEBUG -g -c Renderer.c

HFileIO.o: HFileIO.c HFileIO.h
	gcc -std=c99 -D DEBUG -g -c HFileIO.c

HNotice.o: HNotice.c HNotice.h Renderer.o
	gcc -std=c99 -D DEBUG -g -c HNotice.c

HFlow.o: HFlow.c HFlow.h HNotice.o HGame.o
	gcc -std=c99 -D DEBUG -g -c HFlow.c
