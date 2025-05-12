#ifndef APPSTATE_H
#define APPSTATE_H

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../utils/types.h"

typedef struct AppState_s {
    f64 deltaTime;
    f64 timePassed;
} AppState;

AppState* appState_create();

void appState_setPState(HWND hwnd, LPARAM lParam);
AppState* appState_getPState(HWND hwnd);

void appState_destroy(AppState* appState);



#endif // APPSTATE_H