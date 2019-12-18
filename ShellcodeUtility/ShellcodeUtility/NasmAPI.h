#pragma once

#include <Windows.h>

#define NASM_PATH "./nasm.exe"


// checks if nasm exists. It it doesn't, it tries to install it. TRUE on success
BOOL EnsureNASMExists();
