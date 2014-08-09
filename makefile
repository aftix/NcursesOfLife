CC= gcc
SRC= Life.c
CFLAGS= -DNONW
OBJS= $(SRC:.c=.o)
LIBS= -lncurses
OUT = Ncursesoflife

all: obj

obj: $(OBJS)
	$(CC) $(CFLAGS) $(IBS) $(OBJS) -o $(OUT)
	rm -rf *.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(OUT) *~
