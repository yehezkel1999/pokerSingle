
#include "random.h"

random::random() { // will only be called once
	srand((unsigned)time(0)); 
}

int random::iRandInt(int from, int to) {
	return from + (rand() % (to - from + 1));
}
