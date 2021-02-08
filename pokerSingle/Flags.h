
#ifndef FLAGS_H_
#define FLAGS_H_

#ifdef _DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif // _DEBUG



#define JUST_BOTS 1

#define MEMORY_FLAG 0



#if MEMORY_FLAG
#include "MemoryTracking.h"
#endif


#endif // !FLAGS_H_
