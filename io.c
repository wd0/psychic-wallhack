#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/time.h>
#include "types.h"
#include "tic.h"

static const char *const BOARD_SEP = " | ";
static const char NOTHING_C = ' ';

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
    if (fgets(buf, bufsize, stdin) == NULL)
        exit(DRAW);
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
    return !(toupper(m) == XMARK || toupper(m) == OMARK);
}

enum board
ask_mark(void) {
    int mark;
    int c;
get_mark:
    printf("Which mark? (%c | %c)\n", XMARK, OMARK);
    mark = getchar();
    if ((c = getchar()) != '\n' && c != EOF)
        ungetc(c, stdin);
    if (invalid_mark(mark)) {
        printf("Surely you can't mean to put ``%c'' on the board.\n", mark);
        goto get_mark;
    }
    return toupper(mark);
}

unsigned
millisleep(unsigned milliseconds) {
    struct timeval t = { .tv_sec = 0, .tv_usec = milliseconds * 1e3 };
    select(0, NULL, NULL, NULL, &t);
    return milliseconds;
} 

unsigned
bot_ask_coord(enum board *b, size_t board_size) {
    unsigned coord;
    printf("Okay bot, where do you want to mark?\n");
    do {
        coord = rand() % board_size;
    } while (occupied(b, coord));
    millisleep(400);
    printf("%u\n", coord + 1); 
    return coord;
}

void
show_winner(enum board *b, size_t nrows, size_t ncols, enum board human_mark) {
    int winner = get_winner(b, nrows, ncols, human_mark);
    if (winner == DRAW)
        printf("It's a draw.\n");
    else if (winner == WIN)
        printf("You won with %c!\n", human_mark);
    else if (winner == LOSE)
        printf("You lost with %c!\n", human_mark);
    else
        assert(false);
}

void
show_board(enum board *b, size_t nrows, size_t ncols) {
    const char *sep = BOARD_SEP;
    const unsigned char nothing = NOTHING_C;
    unsigned row, col;
    putchar('\n');
    for (row = 0; row < nrows; ++row) { 
        for (col = 0; col < ncols; ++col) {
            unsigned coord = trans(row, col, nrows);
            putchar(b[coord] == NOTHING? nothing : b[coord]);
            if (col != ncols - 1)
                printf("%s", sep);
        }
        putchar('\n');
    }
    putchar('\n');
}

void
show_helper_board(size_t nrows, size_t ncols) {
    const char *sep = BOARD_SEP;
    unsigned row, col;
    putchar('\n');
    for (row = 0; row < nrows; ++row) { 
        for (col = 0; col < ncols; ++col) {
            printf("%d", trans(row, col, nrows) + 1);
            if (col != ncols - 1)
                printf("%s", sep);
        }
        putchar('\n');
    }
    putchar('\n');
}
