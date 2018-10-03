/*
 * test-32bit.h
 *
 * (C) Simon Gleissner, simon@gleissner.de
 */

#ifndef TEST_32BIT_H_
#define TEST_32BIT_H_

#include <cstdint>

#include <asyncfw/atomic_aba_wrapper.h>

class Test32Bit : public AsyncFW::ABA_Wrapper<int32_t>
{
public:
	Test32Bit(int32_t init = 0)
	: ABA_Wrapper<int32_t>(init)
	{}

	void set(int32_t const value);
	int32_t get()
	{
		return this->read();
	}

#if defined(ATOMIC_DWCAS_NEEDED)
	uint32_t get_counter()
	{
		return this->read_counter();
	}
#endif
};

void Test32Bit::set(int32_t const value)
{
	bool (*inlined_modify_func)(const int32_t&, int32_t&, const int32_t) =	// implicit conversion
			[](const int32_t& /* ptr_cached */, int32_t& value_modify, const int32_t value_init) -> bool
			{
				value_modify = value_init;
				return true;
			};

	this->modify(inlined_modify_func, value);
}

#endif /* TEST_32BIT_H_ */
