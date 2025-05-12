#include "appstate.h"

#include <stdlib.h>

AppState* appState_create() {
    AppState* appState = (AppState*)malloc(sizeof(AppState));
    if (appState == NULL) {
        return NULL;
    }

    // Initialize the AppState members
    appState->deltaTime = 0.0;
    appState->timePassed = 0.0;

    return appState;
}

void appState_setPState(HWND hwnd, LPARAM lParam) {
    CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
    AppState* appState = (AppState*)cs->lpCreateParams;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)appState);
}

AppState* appState_getPState(HWND hwnd) {
    LONG_PTR pStatePtr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    AppState* appState = (AppState*)pStatePtr;
    if (appState == NULL) {
        // Handle error: AppState not set
        return NULL;
    }
    return appState;
}

void appState_destroy(AppState* appState) {
    if (appState != NULL) {
        free(appState);
    }
}