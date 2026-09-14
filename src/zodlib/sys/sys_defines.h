#pragma once

#define MAX_STRING_CHARS        1024
#define MAX_PRINT_MESSAGE_CHARS (1024 * 10)

#if defined(_DEBUG)
// On debug builds, zero out memory has a different value OxCD
#define ZERO_MEMORY 0xCD
#else
#define ZERO_MEMORY 0
#endif

#define CONCAT_(a, b) a##b
#define CONCAT(a, b)  CONCAT_(a, b)
