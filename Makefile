CC = gcc -Wall -O2 -g -ggdb
INSTALL_DIR = /usr/local/sbin

all: redirect

trafcount: Makefile redirect.c
	$(CC) -o redirect redirect.c

install: redirect
	install -o root -s redirect $(INSTALL_DIR)

clean:
	rm -f redirect
