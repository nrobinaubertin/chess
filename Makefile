CC=clang
CFLAGS=-DNDEBUG -O3 -Wall -Wno-unused-result -std=c11 -pedantic -Wno-return-stack-address
# debug flags
CDFLAGS=-ggdb -Wall -Wno-unused-result -std=c11 -pedantic -Wno-return-stack-address
LDFLAGS=

all: chess

chess: chess.o board.o print.o move.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf *.o && rm chess

%.o: %.c
	$(CC) -o $@ -c $< $(CDFLAGS)
