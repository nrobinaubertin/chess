CC=clang
CFLAGS=-DNDEBUG -O3 -Wall -Wno-unused-result -std=c11 -pedantic -Wno-return-stack-address -I./include
# debug flags
CDFLAGS=-ggdb -Wall -Wno-unused-result -std=c11 -pedantic -Wno-return-stack-address -I./include
LDFLAGS=

src = $(wildcard src/*.c) $(wildcard src/structs/*.c)
obj = $(src:.c=.o)

all: chess

chess: $(obj)
	$(CC) $(LDFLAGS) -o $@ $^

#$(obj): $(src)
#	$(CC) -o $@ -c $< $(CDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CDFLAGS)

clean:
	rm -f $(obj) chess

.PHONY: clean all
