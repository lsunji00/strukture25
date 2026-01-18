#ifndef IO_H
#define IO_H

#include "fighter.h"

Fighter* load_fighters(const char* filename);
int load_fights(const char* filename, Fighter* fighters);

#endif
