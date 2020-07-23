#include <switch.h>
#include "stdbool.h"

bool RunMainLoop() {
    return appletMainLoop();
}

void UpdateInputs() {
    hidScanInput();
}

void UpdateConsole() {
    consoleUpdate(NULL);
}