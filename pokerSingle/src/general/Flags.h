
#ifndef FLAGS_H_
#define FLAGS_H_

#ifdef _DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif // _DEBUG

#if DEBUG

#define JUST_BOTS 1
#define MEMORY_FLAG 0
#define OUTPUT_FILE_NAME "log\\output.log"

#if MEMORY_FLAG
#define MEMORY_FILE_NAME "log\\poker_memory_usage.log"
#include "..\tools\MemoryTracking.h"
#endif // MEMORY_FLAG

#endif // DEBUG

typedef unsigned int chips_t; // type used for chips
typedef int schips_t; // signed type used for chips
typedef unsigned short id_t; // type used for ids

#endif // !FLAGS_H_
