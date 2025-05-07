#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>

#ifdef DEBUG
#define ERR_LOG(x) printf("ERR : File %s, Line %d," x, __FILE__, __LINE__)
#define SDL_ERR_LOG(x) ERR_LOG("SDL"x)
#else
#define ERR_LOG(x) 
#define SDL_ERR_LOG(x)
#endif

#define TODO

#define LOG_FPS

#endif