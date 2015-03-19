#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "types.h"
#include "io.h"

#define BOARD_HEIGHT 3 /* Replaced in the makefile */
enum { BOARD_H = BOARD_HEIGHT, BOARD_W = BOARD_H };
enum { BOARD_SIZE = BOARD_H * BOARD_W }; 

unsigned
trans(unsigned row, unsigned col, size_t board_height) {
    return row * board_height + col; 
} 

bool
occupied(enum board *b, unsigned coord) {
    if (b[coord] == NOTHING)
        return false;
    return true;
}

static bool
board_full(enum board *b, size_t board_size) {
    size_t i;
    for (i = 0; i < board_size; ++i) 
        if (!occupied(b, i))
            return false;
    return true;
}

static void
update_board(enum board *b, unsigned coord, enum board mark) {
    b[coord] = mark;
}

static enum board
check_horizontals(enum board *b, size_t nrows, size_t ncols) {
    unsigned row, col;
    enum board mark;
    for (row = 0; row < nrows; ++row) {
        mark = b[trans(row, 0, nrows)];
        for (col = 1; col < ncols; ++col)
            if (mark != b[trans(row, col, nrows)])
                break;
        if (col == ncols) 
            return mark;
    }
    return NOTHING;
}

static enum board
check_verticals(enum board *b, size_t nrows, size_t ncols) {
    unsigned row, col;
    enum board mark;
    for (col = 0; col < ncols; ++col) {
        mark = b[trans(0, col, nrows)];
        for (row = 1; row < nrows; ++row)
            if (mark != b[trans(row, col, nrows)])
                break;
        if (row == nrows) 
            return mark;
    }
    return NOTHING;
}

static enum board
check_diagnonals(enum board *b, size_t nrows, size_t ncols) {
    unsigned row, col;
    enum board mark;
    mark = b[trans(0, 0, nrows)];
    for (row = 1, col = 1; row < nrows && col < ncols; ++row, ++col) 
        if (mark != b[trans(row, col, nrows)]) 
            break;
    if (row == nrows && col == ncols)
        return mark;

    mark = b[trans(0, ncols - 1, nrows)];
    for (row = 1, col = ncols - 2; row < nrows && col > 0; ++row, --col) 
        if (mark != b[trans(row, col, nrows)]) 
            break;
    if (row == nrows - 1 && col == 0 && mark == b[trans(row, 0, nrows)])
        return mark;
    return NOTHING;
}

static int
get_winner_mark(enum board *b, size_t nrows, size_t ncols) {
    enum board winner_mark = 0;
    winner_mark |= check_horizontals(b, nrows, ncols);
    winner_mark |= check_verticals(b, nrows, ncols);
    winner_mark |= check_diagnonals(b, nrows, ncols); 
    return winner_mark; 
}

int
get_winner(enum board *b, size_t nrows, size_t ncols, enum board human_mark) {
    enum board winner_mark = get_winner_mark(b, nrows, ncols);
    if (winner_mark == XMARK) {
        if (human_mark == winner_mark) 
            return WIN;
        else
            return LOSE;
    }
    if (winner_mark == OMARK) {
       if (human_mark == winner_mark)
          return WIN;
      else
         return LOSE;
    } 
    return DRAW;
}

static bool
game_over(enum board *b, size_t nrows, size_t ncols) {
    return get_winner_mark(b, nrows, ncols) != NOTHING || board_full(b, nrows * ncols);
}

/* The player who uses 'X' always goes first. */
static int
tic(enum board *b, size_t nrows, size_t ncols) {
    const size_t board_size = nrows * ncols;
    enum board human_mark = ask_mark();
    enum board bot_mark = human_mark == XMARK? OMARK : XMARK;
    bool human_first = human_mark == XMARK;
    srand(time(NULL));
    show_helper_board(nrows, ncols);
    for (;;) { 
        unsigned coord;
        coord = human_first? ask_coord(b, board_size) : bot_ask_coord(b, board_size);
        update_board(b, coord, human_first? human_mark : bot_mark);
        show_board(b, nrows, ncols);
        if (game_over(b, nrows, ncols))
            break;
        coord = human_first? bot_ask_coord(b, board_size) : ask_coord(b, board_size);
        update_board(b, coord, human_first? bot_mark : human_mark);
        show_board(b, nrows, ncols);
        if (game_over(b, nrows, ncols))
            break;
    }
    show_winner(b, nrows, ncols, human_mark);
    return get_winner(b, nrows, ncols, human_mark);
}

int
main(void) {
    enum board b[BOARD_SIZE] = { NOTHING };
    return tic(b, BOARD_H, BOARD_W);
}
