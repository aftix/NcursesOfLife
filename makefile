CC= gcc
SRC= Life.c
CFLAGS= -DNONW -std=c11 -g
OBJS= $(SRC:.c=.o)
LIBS= -lncurses
OUT = Ncursesoflife

all: obj

obj: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(OUT)
	rm -rf *.o

.c.o:
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

clean:
	rm -rf *.o $(OUT) *~
