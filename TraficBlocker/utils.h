#pragma once
#include <stdio.h>
#define SHOW_ERROR(errorString, ...) {printf("\033[0;31m"); printf(errorString, __VA_ARGS__); printf("\033[0m"); }
#define SHOW_SUCCESS(errorString, ... ){printf("\033[0;32m"); printf(errorString, __VA_ARGS__); printf("\033[0m"); }
#define SHOW_CRASHED(errorString, ... ){printf("\033[0;35m"); printf(errorString, __VA_ARGS__); printf("\033[0m"); }