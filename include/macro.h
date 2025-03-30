#ifndef MACRO_H
#define MACRO_H

void __err_log(const char* log);
void __sdl_err_log(const char* log);

#ifdef DEBUG
#define ERR_LOG(x) __err_log(x)
#define SDL_ERR_LOG(x) __sdl_err_log(x)
#else
#define ERR_LOG(x) 
#define SDL_ERR_LOG(x)
#endif

#define nullptr NULL;
#define TODO

// #define LOG_FPS

#endif