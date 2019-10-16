CC=clang
CFLAGS=-Wall -Wno-unused-result -std=c11 -pedantic -Wno-return-stack-address -I./include
DFLAGS=-ggdb
RFLAGS=-DNDEBUG -O3
LFLAGS=

src = $(wildcard src/*.c) $(wildcard src/structs/*.c)
obj = $(src:.c=.o)

debug: CFLAGS+=$(DFLAGS)
debug: chess

release: CFLAGS+=$(RFLAGS)
release: chess

chess: $(obj)
	$(CC) $(LFLAGS) -o $@ $^

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(obj) chess

.PHONY: clean all debug release
