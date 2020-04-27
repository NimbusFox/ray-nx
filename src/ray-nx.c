//
// Created by nimbusfox on 4/26/20.
//

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