//============================================================================
// Name        : LowLevelAtomic.cpp
// Author      : Simon Gleissner
// Version     :
// Copyright   : (c) Simon Gleissner 2017
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <asyncfw/atomic_concurrent_pointer.h>

#include "test/test_32bit.h"

int main()
{
	AsyncFW::ConcurrentPtr<uint16_t> test;

	std::cout << "Alignment: " << AsyncFW::ConcurrentPtr<uint16_t>::ATOMIC_ALIGNMENT << std::endl;

	uint16_t x1 = 42;
	uint16_t x2 = 4711;
	uint16_t x3 = 0x0815;

	test.set(&x1);
	test.set(&x2);
	test.set(&x3);


	uint16_t* y = test.get();

	std::cout << *y << " " << test.get_counter() << std::endl;

	Test32Bit test32(1234567890);

	test32.set(-2000000000);

	std::cout << "32-Bit Test: "<< test32.get() << " " << test32.get_counter() << std::endl;


	return 0;
}

