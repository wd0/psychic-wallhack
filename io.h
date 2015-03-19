#ifndef IO_H
#define IO_H
#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
unsigned ask_coord(enum board *, size_t);
enum board ask_mark(void);
unsigned bot_ask_coord(enum board *, size_t);
void show_winner(enum board *, size_t, bool);
void show_board(enum board *, size_t, size_t);
#endif
