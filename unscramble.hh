//
// Created by danilo on 4/1/16.
//

#ifndef SCRAMBLE_UNSCRAMBLE_H
#define SCRAMBLE_UNSCRAMBLE_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

#ifndef RELEASE

#define unsc_assert(exp) do { if (!(exp)) { \
	fprintf(stderr, "Error: %s: %d: %s\n", __FILE__, __LINE__, strerror(errno)); \
	exit(EXIT_FAILURE); \
} } while (0) \

#define unsc_logmsg(vargs, ...) do { \
        fprintf(stdout, "Info: %s: %d: " vargs, __FILE__, __LINE__, ##__VA_ARGS__); \
} while (0) \

#else

#define unsc_assert(exp)

#define unsc_logmsg(exp)

#endif

#endif //SCRAMBLE_UNSCRAMBLE_H
