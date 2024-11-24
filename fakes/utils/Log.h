#pragma once

#define ALOGE(...)
#define ALOGV(...)

#if defined(__APPLE__) && defined (__APPLE_CC__)
#include <fcntl.h>
#include <sys/types.h>
typedef __int64_t off64_t;
#endif

#if defined(__WIN32__)
#define OPEN_FLAG _O_BINARY
#elif defined(__linux__)
#define OPEN_FLAG O_LARGEFILE
#else
#define OPEN_FLAG O_CREAT
#endif
