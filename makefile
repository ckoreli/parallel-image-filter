CC = clang
CFLAGS = -Wall

all: imfilter

imfilter: imfilter.c imgparse.c argparse.c message.c utils.c; $(CC) $(CFLAGS) $^ -o $@

clean: ; rm *.o *.out imfilter
