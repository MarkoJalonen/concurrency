#ifndef CONFIG_HH
#define CONFIG_HH

#include <stdint.h>

const struct {
    uint64_t DIV;
    uint64_t ADD;
} CONFIG = { 2, 0xCAFE };


#endif
