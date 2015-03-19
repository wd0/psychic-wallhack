#ifndef TIC_H
#define TIC_H
#include <stdbool.h>
#include <stdlib.h>
#include "types.h"
unsigned trans(unsigned, unsigned, size_t);
bool occupied(enum board *, unsigned);
int get_winner(enum board *, size_t, size_t, enum board);
#endif
