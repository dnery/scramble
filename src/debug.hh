/*
 * Created by danilo on 4/1/16. 
 */

#ifndef SCRAMBLE_DEBUG_H
#define SCRAMBLE_DEBUG_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

#ifndef RELEASE

#define put(vargs, ...) do { \
        fprintf(stdout, "Info: " vargs, ##__VA_ARGS__); \
} while (0) \

#define err(vargs, ...) do { \
        fprintf(stderr, "Error: " vargs, ##__VA_ARGS__); \
} while (0) \

#define check(exp) do { if (!(exp)) { \
	err("%s: %d: %s\n", __FILE__, __LINE__, strerror(errno)); \
	exit(errno); \
} } while (0) \

#else

#define put(exp)

#define err(exp)

#define check(exp)

#endif

#endif /* SCRAMBLE_DEBUG_H */
