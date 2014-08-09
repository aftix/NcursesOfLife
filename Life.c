#include <stdio.h>
#include <stdlib.h>
#include "includes.h"

struct cell(
	int state;
);

typedef struct cell Cell;

void init(Cell **grid);
void sim(Cell **grid);

int main() {
	initscr();
	erase();
	keypad(stdscr, 1);
	noecho();
	cbreak();
	curs_set(0);
	
	Cell **gameGrid = malloc(LINES);
	
	for (int i=0; i<LINES; i++)
		gameGrid[i] = malloc(COLS);

	init(gameGrid);
	sim(gameGrid);
		
	endwin();
	
	for (int i=0; i<LINES; i++)
		free(gameGrid[i]);

	free(gameGrid);
	return 0;
}

void init(Cell **grid) {
	curs_set(0);
	for (int i=0; i<LINES; i++) {
		for (int j=0; j<COLS; j++) {
			grid[i][j].state = 0;
		}
	}

	int y=0, x=0;
		
	timeout(10);
	while (1) {
		char ch = getch();
		
		if (ch == 'q') return;
		
		if (ch == 'w') y--;
		if (ch == 's') y++;
		if (ch == 'a') x--;
		if (ch == 'd') x++;

		if (y > LINES-1) y = LINES-1;
		if (x > COLS-1) x = COLS-1;
		if (y < 0) y = 0;
		if (x < 0) x = 0;

		if (ch == '#') grid[y][x].state = 1;
		if (ch == ' ') grid[y][x].state = 0;
	}	
}

void sim(Cell **grid) {
	curs_set(0);
}
