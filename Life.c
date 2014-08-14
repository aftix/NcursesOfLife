#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"

typedef struct cell {
	int state;
} Cell;

void init(Cell *grid);
void sim(Cell *grid);

int main() {
	initscr();
	erase();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);
	refresh();
	
	Cell *gameGrid = malloc(LINES * COLS * sizeof(Cell));
	
	init(gameGrid);
	sim(gameGrid);
		
	endwin();

	free(gameGrid);
	return 0;
}

void init(Cell *grid) {
	curs_set(1);
	for (int i=0; i<LINES; i++) {
		for (int j=0; j<COLS; j++) {
			grid[i*LINES + j].state = 0;
		}
	}

	int y=0, x=0;
		
	timeout(10);
	while (1) {
		int ch = getch();
		
		if (ch == 'q') return;
		
		if (ch == 'w' || ch == KEY_UP) y--;
		if (ch == 's' || ch == KEY_DOWN) y++;
		if (ch == 'a' || ch == KEY_LEFT) x--;
		if (ch == 'd' || ch == KEY_RIGHT) x++;

		if (y > LINES-1) y = LINES-1;
		if (x > COLS-1) x = COLS-1;
		if (y < 0) y = 0;
		if (x < 0) x = 0;

		if (ch == '#' || ch == '3') {
			grid[y*LINES + x].state = 1;
			move(y,x);
			addch('#');
		}

		if (ch == ' ') {
			grid[y*LINES + x].state = 0;
			move(y, x);
			addch(' ');
		}
		
		move(y, x);
	}
}

void sim(Cell *grid) {
	curs_set(0);

	timeout(-1);
	int ch;
	Cell *next = malloc(LINES*COLS*sizeof(Cell));

	while (1) {
		if (ch == 'q') {
			free(next);
			return;
		}

		memcpy(next, grid, sizeof(Cell)*LINES*COLS);

		int count = 0;
		for (int i=0; i<LINES; i++) {
			for (int j=0; j<COLS; j++) {
				if (j > 0) count += grid[i*LINES + j - 1].state;
				if (j < COLS-1) count += grid[i*LINES + j + 1].state;
				if (i > 0) count += grid[(i-1)*LINES + j].state;
				if (i < LINES-1) count += grid[(i+1)*LINES + j].state;
				if (j > 0 && i > 0) count += grid[(i-1)*LINES + j - 1].state;
				if (j > 0 && i < LINES-1) count += grid[(i+1)*LINES + j - 1].state;
				if (j < COLS-1 && i > 0) count += grid[(i-1)*LINES + j + 1].state;
				if (j < COLS-1 && i < LINES-1) count += grid[(i+1)*LINES + j + 1].state;
				
				if (count == 3) next[i*LINES + j].state = 1;
				if (count < 2) next[i*LINES + j].state = 0;
				if (count > 3) next[i*LINES + j].state = 0;			
			}
		}

		memcpy(grid, next, sizeof(Cell)*LINES*COLS);

		for (int i=0; i<LINES; i++) {
			for (int j=0; j<COLS; j++) {
				move(i, j);
				addch(grid[i*LINES + j].state == 0 ? ' ' : '#');
			}
		}

		ch = getch();
	}
}
