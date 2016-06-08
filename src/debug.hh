#ifndef SCRAMBLE_DEBUG_H
#define SCRAMBLE_DEBUG_H

/*
 * This is a shitty debug functionality header for my C++ projects.
 *
 * This little fucker right here has to be included after everything else,
 * anywhere you wish to use it (Meaning LAST and ONLY ON IMPLEMENTATION FILES).
 *
 * I might fix this sometime. Or not...
 */

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#ifndef RELEASE

#define put(vargs, ...) do { \
        std::fprintf(stdout, vargs, ##__VA_ARGS__); \
} while (0) \

#define err(vargs, ...) do { \
        std::fprintf(stderr, "Error: " vargs, ##__VA_ARGS__); \
} while (0) \

#define info(vargs, ...) do { \
        std::fprintf(stdout, "Info: " vargs, ##__VA_ARGS__); \
} while (0) \

#define warn(exp, vargs, ...) do { if (!(exp)) { \
        std::fprintf(stdout, "Warning: " vargs, ##__VA_ARGS__); \
} } while (0) \

#define check(exp) do { if (!(exp)) { \
        err("%s: %d: %s\n", __FILE__, __LINE__, std::strerror(errno)); \
        std::exit(errno); \
} } while (0) \

#else

#define put(exp)

#define err(exp)

#define info(exp)

#define warn(exp)

#define check(exp)

#endif

#endif /* SCRAMBLE_DEBUG_H */
