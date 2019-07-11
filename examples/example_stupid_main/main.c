#include <stdlib.h>
#include <limits.h>

/** Benchmarking::User::Timer(declare) */

/** Benchmarking::User::Monitor(declare) */

// TODO for lazy people but not advised Benchmarking::User::ReplaceInCode(#include <stdio.h>)

int main(int argc, char **argv)
{
	unsigned int i = 0;
	char data[128];

	/** Benchmarking::User::Timer(start,timer1) */

	/** Benchmarking::User::Monitor(U32, i) */
	for (; i < UINT_MAX; i++) {
		data[i % sizeof(data)] = i % ((1 << (sizeof(char) * 8)) - 1);
	}

	/** Benchmarking::User::Monitor(U32, i) */

	/** Benchmarking::User::Timer(stop,timer1) */

	return 0;
}
