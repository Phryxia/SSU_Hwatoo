#ifndef HGUI
#define HGUI

#include <stdbool.h>
#include "HCard.h"

//#define setCursor(x, y) printf("\e["#y";"#x"f")

#define FDCOLOR(x) "\e[3"#x"m"
#define FBCOLOR(x) "\e[9"#x"m"
#define BDCOLOR(x) "\e[4"#x"m"
#define BBCOLOR(x) "\e[10"#x"m"

#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7  
#define RESET   9

#define FOREGROUND 0
#define BACKGROUND 1
#define DARK       0
#define BRIGHT     1

#define ALIGN_LEFT   0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT  2

int  HGUI_getch(void);
void HGUI_cSet(int color, int mode, int brightness);
void HGUI_cReset(void);
void HGUI_erase(void);
void HGUI_curSet(int xpos, int ypos);

void HGUI_rect(int x1, int y1, int x2, int y2);
void HGUI_window(int x1, int y1, int x2, int y2);
void HGUI_text(int xpos, int ypos, char const *text, bool wide, int mode); // Center Allign

int  HGUI_menu(int xpos, int ypos, char const **, int m_length); // Center Biased xpos.
void HGUI_card(int xpos, int ypos, HCard const *card);

#define CARD_WIDTH  5
#define CARD_HEIGHT 5

#endif