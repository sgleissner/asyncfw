/*
 * asyncfw_helper.h
 *
 * (C) Simon Gleissner, simon@gleissner.de
 */

#ifndef ASYNCFW_HELPER_H_
#define ASYNCFW_HELPER_H_

// branch hint macros
// see: https://stackoverflow.com/questions/30130930/is-there-a-compiler-hint-for-gcc-to-force-branch-prediction-to-always-go-a-certa
#define UNLIKELY(cond) __builtin_expect(static_cast<bool>(cond), false)
#define   LIKELY(cond) __builtin_expect(static_cast<bool>(cond), true)

// force inline code
#define ALWAYS_INLINE inline __attribute__((always_inline))

// count leading zeroes
#define CLZ(x) __builtin_clz(x)		/* x must not be 0 */

// platform helpers
#if defined(__x86_64__) || defined(__aarch64__)
#define PLATFORM_64
#elif defined(__i686__) || defined(__arm__)
#define PLATFORM_32
#else
#error "unsupported platform"
#endif

#endif /* ASYNCFW_HELPER_H_ */
