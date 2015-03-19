bin = tic
src = tic.c io.c
cflags = -Wall -Wextra -pedantic -std=c99

all: build 

test: build
	./${bin}

build: 
	cc -o ${bin} ${src} ${cflags}

install:
	sudo install -d ${bin} "/usr/local/bin/"

uninstall:
	sudo rm -f "/usr/local/bin/${bin}"
