/*** CLIC is a library for creating Command Line Interfaces in the C language. ***/
/**
 * Paulo Regis M. Sousa
 */
#ifndef CLIC_H
#define CLIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define Symbol_HLINE "\u2501"
#define Symbol_VLINE "\u2503"
#define Symbol_CORNER_TL "\u250f"
#define Symbol_CORNER_TR "\u2513"
#define Symbol_CORNER_BL "\u2517"
#define Symbol_CORNER_BR "\u251b"
#define Symbol_ARROW "\u2192"
#define Symbol_ARROW_RETURN "\u21b3"
#define Symbol_T_SIDE_L "\u2523"
#define Symbol_T_SIDE_R "\u252b"
#define Symbol_T_UP "\u2533"
#define Symbol_T_DOWN "\u253b"
#define Symbol_CROSS "\u254b"
#define Symbol_TREE_POINTS "\u2026"
#define Symbol_DIAG "\u2572"
#define Symbol_CDIAG "\u2571"
#define Symbol_BLOCK "\u2588"
#define Symbol_SAD "\U0001f641"
#define Symbol_SMILE "\U0001f642"

typedef enum   Key    Key;
typedef struct Format Format;
typedef struct Cell   Cell;
typedef struct Row    Row;
typedef struct Table  Table;

typedef enum Color {
	Color_BLACK     = 0,
	Color_GRAY      = 8,
    Color_RED       = 9,
	Color_GREEN     = 10,
	Color_YELLOW    = 11,
	Color_MAGENTA   = 13,
	Color_CIAN      = 14,
	Color_WHITE     = 15,
	Color_BLUE      = 39,
	Color_ORANGE    = 208,
	Color_LIGHTGRAY = 248
} Color;

void Clic_resetColor();
void Clic_setFontColor(Color color);
void Clic_setBackgroundColor(Color color);

void Clic_move(int line, int column);
void Clic_moveUp(int lines);
void Clic_moveDown(int lines);
void Clic_moveRight(int columns);
void Clic_moveLeft(int columns);
void Clic_moveDownBegin(int lines);
void Clic_moveUpBegin(int lines);
void Clic_moveToColumn(int column);
void Clic_moveToBegin();

void Clic_clearLine();
void Clic_breakLine();

void Clic_printSymbol(char *symbol);
void Clic_printHLine(int width);
void Clic_printVLine(int height);
void Clic_printHBlockLine(int width);
void Clic_printVBlockLine(int height);
int  Clic_printCenter(char *text);

void Clic_saveCursorPosition();
void Clic_restoreCursorPosition();
void Clic_hideCursor();
void Clic_showCursor();
void Clic_getCursorPosition(int *row, int *col);

void Clic_clearScreen();
int  Clic_getScreenWidth();
int  Clic_getScreenHeight();

void Clic_printBox(int width, int height);
void Clic_fixDraw();

int  Clic_keyCapture();
void Clic_pause(float seconds);

Table *Table_create(char *format);
void   Table_setWidth(Table *table, int width);
void   Table_free(Table *table);
void   Table_addRow(Table *table, ...);
void   Table_print(Table *table);
int    Table_select(Table *table);

#define Row_MAX_CELLS 60

struct Format {
	char formatString[30];
	bool isLeftAligned;
	bool isCustomWidth;
	int  width;
};

struct Cell {
	char *content;
	Format format;
};

struct Row {
    int index;
    int length;
    Cell cell[Row_MAX_CELLS];

    Row *next;
    Row *prev;
};

struct Table {
	int nRows;
	int nCols;
	int width;
	int defaultCellWidth;
    int highlightedIndex;
    Color highlightColor;
	Format *formats;

	void *(*selectCallback)(Row *row);
	void *(*updateCallback)(Row *row);
	void *(*deleteCallback)(Row *row);
    
    Row *firstRow;
    Row *lastRow;
};

enum Key {
	Key_TAB    = 9,   
    Key_ENTER  = 10,
    Key_SPACE  = 32,
    Key_0      = 48,
    Key_1      = 49,
    Key_2      = 50,
    Key_3      = 51,
    Key_4      = 52,
    Key_5      = 53,
    Key_6      = 54,
    Key_7      = 55,
    Key_8      = 56,
    Key_9      = 57,
	Key_UP     = 65,
	Key_DOWN   = 66,
	Key_RIGHT  = 67,
	Key_LEFT   = 68,
	Key_A      = 97,
	Key_B      = 98,
	Key_C      = 99,
	Key_D      = 100,
	Key_E      = 101,
	Key_F      = 102,
	Key_G      = 103,
	Key_H      = 104,
	Key_I      = 105,
	Key_J      = 106,
	Key_K      = 107,
	Key_L      = 108,
	Key_M      = 109,
	Key_N      = 110,
	Key_O      = 111,
	Key_P      = 112,
	Key_Q      = 113,
	Key_R      = 114,
	Key_S      = 115,
	Key_T      = 116,
	Key_U      = 117,
	Key_V      = 118,
	Key_W      = 119,
	Key_X      = 120,
	Key_Y      = 121,
	Key_Z      = 122,
	Key_DELETE = 127
};

#endif /* CLIC_H */
