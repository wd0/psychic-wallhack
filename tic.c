#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "types.h"
#include "io.h"

enum { BOARD_H = 3, BOARD_W = 3};
enum { BOARD_SIZE = BOARD_H * BOARD_W }; /* Must be a perfect square */

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
    unsigned row;
    int col;
    enum board mark;
    mark = b[trans(0, 0, nrows)];
    for (row = 1, col = 1; row < nrows && col < ncols; ++row, ++col) 
        if (mark != b[trans(row, col, nrows)]) 
            break;
    if (row == nrows && col == ncols)
        return mark;

    mark = b[trans(0, ncols - 1, nrows)];
    for (row = 0, col = ncols - 1; row < nrows && col >= 0; ++row, --col) 
        if (mark != b[trans(row, col, nrows)]) 
            break;
    if (row == nrows && col == -1)
        return mark;
    return NOTHING;
}


int
get_winner(enum board *b, size_t board_size) {
    enum board winner = 0;
    unsigned nrows = sqrt(board_size);
    unsigned ncols = nrows;
    winner |= check_horizontals(b, nrows, ncols);
    winner |= check_verticals(b, nrows, ncols);
    winner |= check_diagnonals(b, nrows, ncols); 
    return winner == NOTHING? 0 : winner;
}

static bool
game_over(enum board *b, size_t board_size) {
    return get_winner(b, board_size) != 0 || board_full(b, board_size);
}

/* The player who uses 'X' always goes first. */
static int
tic(enum board *b, size_t nrows, size_t ncols, struct options opts) {
    const size_t board_size = nrows * ncols;
    enum board mark = ask_mark();
    enum board bot_mark = mark == XMARK? OMARK : XMARK;
    bool human_first = mark == XMARK;
    srand(time(NULL));
    for (;;) { 
        unsigned coord;
        coord = human_first? ask_coord(b, board_size) : bot_ask_coord(b, board_size);
        update_board(b, coord, human_first? mark : bot_mark);
        show_board(b, nrows, ncols);
        if (game_over(b, board_size))
            break;
        coord = human_first? bot_ask_coord(b, board_size) : ask_coord(b, board_size);
        update_board(b, coord, human_first? bot_mark : mark);
        show_board(b, nrows, ncols);
        if (game_over(b, board_size))
            break;
    }
    show_winner(b, board_size, human_first);
    return 0;
}

int
main(int argc, char **argv) {
    enum board board[BOARD_SIZE] = { NOTHING };
    struct options opts = { 0 };
    return tic(board, BOARD_H, BOARD_W, opts);
}
