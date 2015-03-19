#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include "types.h"
#include "tic.h"

static int
validate_coord(enum board *b, size_t board_size, int coord) {
    if (coord < 0)
        return 1;
    else if ((unsigned)coord >= board_size)
        return 2;
    else if (occupied(b, coord))
        return 3; 
    return 0;
}

unsigned
ask_coord(enum board *b, size_t board_size) {
    int coord;
    enum { bufsize = 20 };
    char buf[bufsize] = { 0 };
    int rc;
get_coord:
    puts("Where do you want to mark?");
    fgets(buf, bufsize, stdin);
    coord = atoi(buf) - 1;
    if ((rc = validate_coord(b, board_size, coord)) != 0) {
        if (rc == 1) 
            printf("%d? Try again with a more natural number!\n", coord + 1);
        else if (rc == 2)
            printf("%d? That's larger than the board!\n", coord + 1);
        else if (rc == 3)
            printf("%d? There's already a mark there!\n", coord + 1);
        else 
            assert(false);
        goto get_coord;
    }
    return coord; 
}

static bool
invalid_mark(char m) {
    return !(m == XMARK || m == OMARK);
}

enum board
ask_mark(void) {
    char mark;
get_mark:
    puts("Which mark?");
    mark = getchar();
    (void)getchar();
    if (invalid_mark(mark)) {
        printf("Surely you can't mean to put ``%c'' on the board.\n", mark);
        goto get_mark;
    }
    return mark;
}

unsigned
bot_ask_coord(enum board *b, size_t board_size) {
    unsigned coord;
    printf("Okay bot, where do you want to mark?\n");
    do {
        coord = rand() % board_size;
    } while (occupied(b, coord));
    sleep(1);
    printf("%u\n", coord + 1); 
    return coord;
}

void
show_winner(enum board *b, size_t board_size, bool human_first) {
    enum board winner_mark = get_winner(b, board_size);
    if (winner_mark == XMARK) {
        if (human_first)
            printf("You won with %c!\n", XMARK);
        else
            printf("You lost with %c!\n", OMARK);
    } 
    if (winner_mark == OMARK) {
        if (human_first)
            printf("You lost with %c!\n", XMARK);
        else
            printf("You won with %c!\n", OMARK);
    } 
    if (winner_mark == NOTHING) 
        printf("Draw!\n");
}

void
show_board(enum board *b, size_t nrows, size_t ncols) {
    const char *sep = " | ";
    const char nothing = ' ';
    unsigned row, col;
    putchar('\n');
    for (row = 0; row < nrows; ++row) { 
        for (col = 0; col < ncols; ++col) {
            unsigned coord = trans(row, col, nrows);
            char c;
            if (b[coord] == NOTHING)
                c = nothing;
            else
                c = b[coord];
            putchar(c);
            if (col != ncols - 1)
                printf("%s", sep);
        }
        putchar('\n');
    }
    putchar('\n');
}
