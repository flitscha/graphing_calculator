#pragma once

#define EXIT_ERROR(...) do { \
    fprintf(stderr, __VA_ARGS__); \
    perror(""); \
    exit(1);\
} while(0);
