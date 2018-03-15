#ifndef __RWOPS_H
#define __RWOPS_H

#include <SDL/SDL.h>

SDL_RWops *rwFromSizedFP(FILE *in, int size, bool autoclose = true);

#endif
