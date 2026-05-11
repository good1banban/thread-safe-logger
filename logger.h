#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>

typedef enum {
    INFO,
    WARN,
    ERROR
} LogLevel;

void start_logger(void);
void get_timestamp(char *buffer, size_t size);
const char *level_to_string(LogLevel level);

#endif