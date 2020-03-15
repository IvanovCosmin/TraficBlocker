#pragma once
#include <stdio.h>
#define SHOW_ERROR(errorString, ...) {printf("\033[0;31m"); printf(errorString, __VA_ARGS__); printf("\033[0m"); }
#define SHOW_SUCCESS(message, ... ){printf("\033[0;32m"); printf(message, __VA_ARGS__); printf("\033[0m"); }
#define SHOW_CRASHED(errorString, ... ){printf("\033[0;35m"); printf(errorString, __VA_ARGS__); printf("\033[0m"); }
#define SHOW_DEBUG(message, ... ) {printf("\033[0;33m"); printf(message, __VA_ARGS__); printf("\033[0m"); }

#define max(a, b) ((a) > (b) ? (a) : (b))