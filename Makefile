PROGS = gen enc syn seq dec eval
DATA = gendata encdata syndna seqdata decdata
CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

all: gen enc syn seq dec eval

.c:
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm  $(PROGS) $(DATA)
