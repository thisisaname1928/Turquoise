#pragma once

#include <cstdint>
enum kernelErrors {
  OUT_OF_MEMORY,
  ACCESS_VIOLENCE,
};

void kernelErrorHandler(uint64_t err, int line, const char *fn,
                        const char *file);

#define fireError(err) kernelErrorHandler(err, __LINE__, __FUNCTION__, __FILE__)