/*
 * atomic_llsc_asm-sc.h
 *
 * (C) Simon Gleissner, simon@gleissner.de
 */

#ifndef ATOMIC_LLSC_ASM_H_
#define ATOMIC_LLSC_ASM_H_

#if defined __arm__ || defined __aarch64__

#include <cstdint>
#include <type_traits>

#define ATOMIC_LLSC_NEEDED

#if !defined(__GCC_ASM_FLAG_OUTPUTS__)
#error "No GCC inline assembler with flag outputs"
#endif

#endif


#if defined(ATOMIC_LLSC_NEEDED)

// "For reasons of performance, keep the number of instructions between corresponding LDREX and STREX instructions to a minimum."
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0204f/Cihbghef.html

// ARM contraints: https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html#Machine-Constraints

namespace AsyncFW
{

#if defined(__aarch64__)

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0802a/LDXR.html

//

inline static T load_link(const uint64_t* const ptr, uint64_t* const target)
{
	// TODO: ldxr
}

inline static bool store_conditional(uint64_t* const ptr, const uint64_t* const store)
{
	// TODO: stxr
	return false;
}

#endif

#if defined(__aarch64__) || defined (__arm__)

#if (__ARM_FEATURE_LDREX & 8)

inline static T load_link(const uint64_t* const ptr, uint64_t* const target)
{
	// In the ARM LDREXD instruction, Rd must be an even numbered register, and not r14.
	// Rt2 must be R(t+1)
	// TODO: ldrexd
}

inline static bool store_conditional(uint64_t* const ptr, const uint64_t* const store)
{
	// In the ARM STREXD instruction, Rm must be an even numbered register, and not r14.
	// Rt2 must be R(t+1)
	// TODO: strexd
	return false;
}

#endif

#if (__ARM_FEATURE_LDREX & 4)

inline static T load_link(const uint32_t* const ptr, uint32_t* const target)
{
	// TODO: ldrex
	asm volatile
	(
		"ldrex %0, [%1]"
		:		"=r" (*target)
		:		"r" (ptr)
		:
	);
}

inline static bool store_conditional(uint32_t* const ptr, const uint32_t* const store)
{
	bool failed;
	// TODO: strex
	asm volatile
	(
		"strex %0, %1, [%2]"
		:		"=r" (failed)
		:		"r" (*store)
		,		"r" (ptr)
		:
	);

	return !failed;
}

#endif

#if (__ARM_FEATURE_LDREX & 2)

inline static T load_link(const uint16_t* const ptr, uint16_t* const target)
{
	// TODO: ldrexh
}

inline static bool store_conditional(uint16_t* const ptr, const uint16_t* const store)
{
	// TODO: strexh
	return false;
}

#endif

#if (__ARM_FEATURE_LDREX & 1)

inline static T load_link(const uint8_t* const ptr, uint8_t* const target)
{
	// TODO: ldrexb
}

inline static bool store_conditional(uint8_t* const ptr, const uint8_t* const store)
{
	// TODO: strexb
	return false;
}

#endif


#endif	// __aarch64__ || __arm__

}		// namespace AsyncFW

#endif	// ATOMIC_LLSC_NEEDED

#endif	// ATOMIC_LLSC_ASM_H_
