bin = tic
src = tic.c io.c
cflags = -Wall -Wextra -std=c99 -lm

all: build

test: build
	./${bin}

build:
	cc -o ${bin} ${src} ${cflags}
