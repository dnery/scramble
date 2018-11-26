/*
 * Simple debug functionality header for C++ projects.
 *
 * Has to be included after everything else, anywhere you wish to use it
 * (Meaning LAST and ONLY ON IMPLEMENTATION FILES).
 *
 * I might fix this sometime.
 */
#ifndef SCRAMBLE_DEBUG_H
#define SCRAMBLE_DEBUG_H

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#ifndef RELEASE

/* compiler will probably complaing about using GNU extensions. */
#define put(vargs, ...) do { \
        std::fprintf(stdout, vargs, ##__VA_ARGS__); \
} while (0) \

#define info(vargs, ...) do { \
        std::fprintf(stdout, "Info: " vargs, ##__VA_ARGS__); \
} while (0) \

/*
 * This isn't accurate, actually.
 *
 * Call to fprintf might change errno, so the error code
 * outputted may be imprecise. favor the message instead.
 */
#define check(exp) do { if ((exp)) { \
        std::fprintf(stdout, "Error: %s: %d: ", __FILE__, __LINE__); \
        std::fprintf(stdout, "%s\n", std::strerror(errno)); \
        std::exit(errno); \
} } while (0) \

#define mcheck(exp, vargs, ...) do { if ((exp)) { \
        std::fprintf(stdout, "Error: %s: %d: ", __FILE__, __LINE__); \
        std::fprintf(stdout, vargs, ##__VA_ARGS__); \
        std::exit(errno); \
} } while (0) \

#else

#define put(exp)

#define info(exp)

#define check(exp)

#define mcheck(exp)

#endif

#endif /* SCRAMBLE_DEBUG_H */
