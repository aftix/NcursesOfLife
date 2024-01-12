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
	// Set up ncurses screen environment
	initscr();
	erase();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);		// set cursor to invisible
	refresh();

	start_color();
	use_default_colors();
	init_pair(1, COLOR_YELLOW, -1);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	// Set the screen dimensions based on current display
	int MAXX, MAXY;
	getmaxyx(stdscr, MAXY, MAXX);
	MAXY -= 1;	

	// Environment grid is created to be a memory block based on the size of the
	// ncurses screen dimensions
	Cell *gameGrid = malloc(MAXY * MAXX * sizeof(Cell));
	
	init(gameGrid);	// One-time initialize the environment
	sim(gameGrid);	// Enter infinte loop processing keyboard input and evolving cells
		
	endwin();		// Close the ncurses window

	free(gameGrid);	// Free allocated memory
	return 0;
}

void init(Cell *grid) {
	curs_set(1);		// set cursor to visible normal
	int MAX_X, MAX_Y;
	getmaxyx(stdscr, MAX_Y, MAX_X);		// get dimensions of the standard screen window
	MAX_Y -=1;			// make vertical dimension one less than the max window

	// Iterate through entire grid one cell at a time and set to zero
	// Grid[] is a linear array of type Cell objects,
	// arranged row to row (end to end), offset pointer determined by sizeof Cell
	// e.g., Grid[] = (0,0),(1,0),(2,0),(3,0),(0,1),(1,1),(2,1),(3,1),(0,2),(1,2),...
	// Iterate a row at a time, then increment to next column offset
	for (int i=0; i<MAX_Y; i++) {			// next row i
		for (int j=0; j<MAX_X; j++) {		// interate by col j in row i
			grid[i*MAX_X + j].state = 0;	// set cell to zero
		}
	}

	erase();		// Clear the displayed window
}

/*
 * Runs the cell evolution as an infinte loop until keystroke stops it
 * 
 */
void sim(Cell *grid) {
	int MAX_X, MAX_Y;
	getmaxyx(stdscr, MAX_Y, MAX_X);
	// Reserve bottom line of screen for displaying current count
	MAX_Y -= 1;
	int curr = 1;	// Set initial cursor state to true

	int ch;
	// Allocate the next evolution cycle grid
	Cell *next = malloc(MAX_Y*MAX_X*sizeof(Cell));

	int speed = 100;

	int y = 0, x = 0;

	int iter = 0;
	
	attroff(COLOR_PAIR(1));
	attron(A_REVERSE | COLOR_PAIR(2));
	// Write "Iteration:" on lower left of screen
	move(MAX_Y, 0);
	addstr("Iteration: ");
	attroff(A_REVERSE | COLOR_PAIR(2));
	attron(COLOR_PAIR(1));

	// Infinite evolutionary loop. Exit on 'q' keystroke.
	while (1) {
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(2) | A_REVERSE);
		char tmp[15];
		sprintf(tmp, "%d", iter);
		// Write current count to lower left of screen at position x=11
		move(MAX_Y, 11);
		addstr(tmp);
		attroff(A_REVERSE | COLOR_PAIR(2));
		attron(COLOR_PAIR(1));
	
		if (ch == 'q') {
			free(next);
			return;
		}

		if (ch == 'p') 
			curr = curr == 1 ? 0 : 1;

		if (ch == '+') 
			speed -= 10;
		if (ch == '-') 
			speed += 10;

		if (ch == 'i') 
			init(grid);

		if (speed < 1) 
			speed = 1;

		if (!curr) 
			curs_set(0);

		// Copy the latest evolution of the grid to the display window
		memcpy(next, grid, sizeof(Cell)*MAX_Y*MAX_X);

		/* Count living neighbors of each cell, moving through the grid one row at a time
		 *	i = row, j = col
		 * 
		 *	Above left: (-1,-1)
		 * 	Above: (0,-1)
		 * 	Above right: (+1,-1)
		 * 	Left: (-1,0)
		 * 	Right: (+1,0)
		 *	Below left: (-1,+1) 
		 * 	Below: (0,+1)
		 * 	Below right: (+1,+1)
		 * 	
		*/
		int count = 0;
		// Column loop
		for (int i=0; i<MAX_Y; i++) {
			// Row loop
			for (int j=0; j<MAX_X; j++) {
				// Left neighbor, (-1,0)
				if (j > 0) count += grid[i*MAX_X + j - 1].state;	// Wrapped
				else count += grid[i*MAX_X + MAX_X-1].state;
				// Right neighbor, (+1,0)
				if (j < MAX_X-1)
					count += grid[i*MAX_X + j + 1].state;
				else 
					count += grid[i*MAX_X].state;
				// 
				if (i > 0) 
					count += grid[(i-1)*MAX_X + j].state;
				else 
					count+= grid[(MAX_Y-1)*MAX_X + j].state;

				if (i < MAX_Y-1) 
					count += grid[(i+1)*MAX_X + j].state;
				else 
					count += grid[j].state;

				if (j > 0 && i > 0) 
					count += grid[(i-1)*MAX_X + j - 1].state;
				else if (j>0 && !(i > 0)) 
					count += grid[(MAX_Y-1)*MAX_X + j-1].state;
				else if (!(j>0) && i > 0)
					count += grid[(i-1)*MAX_X + MAX_X-1].state;
				else if (!(j>0) && !(i>0))
					count += grid[(MAX_Y-1)*MAX_X + MAX_X-1].state;

				if (j > 0 && i < MAX_Y-1)
					count += grid[(i+1)*MAX_X + j - 1].state;
				else if (j > 0 && !(i < MAX_Y-1))
					count += grid[j-1].state;
				else if (!(j > 0) && i < MAX_Y-1)
					count += grid[(i+1)*MAX_X + MAX_X-1].state;
				else if (!(j > 0) && !(i < MAX_Y-1))
					count += grid[MAX_X-1].state;

				if (j < MAX_X-1 && i > 0)
					count += grid[(i-1)*MAX_X + j + 1].state;
				else if (j < MAX_X-1 && !(i > 0))
					count += grid[(MAX_Y-1)*MAX_X + j + 1].state;
				else if (!(j < MAX_X-1) && i > 0)
					count += grid[(i-1)*MAX_X].state;
				else if (!(j < MAX_X-1) && !(i > 0))
					count += grid[(MAX_Y-1)*MAX_X].state;

				if (j < MAX_X-1 && i < MAX_Y-1)
					count += grid[(i+1)*MAX_X + j + 1].state;
				else if (j < MAX_X-1 && !(i < MAX_Y-1))
					count += grid[j+1].state;
				else if (!(j < MAX_X-1) && i < MAX_Y-1)
					count += grid[(i+1)*MAX_X].state;
				else if (!(j < MAX_X-1) && !(i < MAX_Y-1))
					count += grid[0].state;
				
				// Evolve the next iteration of the current cell and store in next grid
				if (count == 3)
					next[i*MAX_X + j].state = 1;
				else if (count < 2)
					next[i*MAX_X + j].state = 0;
				else if (count > 3)
					next[i*MAX_X + j].state = 0;			
				
				count = 0;
			}
		}

		// Copy evolved grid to the display grid
		memcpy(grid, next, sizeof(Cell)*MAX_Y*MAX_X);

		for (int i=0; i<MAX_Y; i++) {
			for (int j=0; j<MAX_X; j++) {
				move(i, j);
				addch(grid[i*MAX_X + j].state == 0 ? ' ' : '#');
			}
		}
 
		iter++;		// Evolution count increment
		// If user has not pressed a key, i.e., curr != 1, set speed and get current character
		if (!curr) {
			timeout(speed);
			ch = getch();
		} else {			// Check key pressed
			timeout(10);
			while (1) {
				attron(A_REVERSE);
				curs_set(1);		// Make cursor visible	

				ch = getch();		// Get key pressed
				
				if (ch == 'w' || ch == KEY_UP)
					y--;
				if (ch == 's' || ch == KEY_DOWN)
					y++;
				if (ch == 'a' || ch == KEY_LEFT)
					x--;
				if (ch == 'd' || ch == KEY_RIGHT)
					x++;

				if (ch == 'n')
					break;
				
				if (ch == 'p') {
					curr = 0;
					ch = ' ';
					break;
				}

				if (ch == 'q') {
					free(next);
					return;
				}

				if (ch == '+')
					speed -= 10;
				if (ch == '-')
					speed += 10;

				// Don't run out of grid array bounds vertical(Y) or horizontal(X)
				if (y > MAX_Y-1)
					y = MAX_Y-1;
				if (x > MAX_X-1)
					x = MAX_X-1;
				if (y < 0)
					y = 0;
				if (x < 0)
					x = 0;
				
				// User pressed space, toggle cell alive to dead or dead to alive
				if (ch == ' ') {
					// If current cell is alive (i.e., non-zero, kill it by setting to zero
					// Otherwise keep alive by setting to 1
					grid[y*MAX_X + x].state = grid[y*MAX_X + x].state ? 0 : 1;
					move(y, x);	// move cursor to next cell
					// Insert the state chrarcter at current cursor position
					addch(grid[y*MAX_X + x].state ? '#' : ' ');
				}
		
				move(y, x);		//  move cursor to selected cell
			}
		}
	}
}
