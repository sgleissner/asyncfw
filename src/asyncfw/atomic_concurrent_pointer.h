/*
 * atomic_concurrent_pointer.h
 *
 * (C) Simon Gleissner, simon@gleissner.de
 */

#ifndef ATOMIC_CONCURRENT_POINTER_H_
#define ATOMIC_CONCURRENT_POINTER_H_

#include <cstdint>

#include <asyncfw/atomic_aba_wrapper.h>

namespace AsyncFW
{

template<typename T>
class ConcurrentPtr : public ABA_Wrapper<T*>
{
public:
	ConcurrentPtr(T* init = nullptr)
	: ABA_Wrapper<T*>(init)
	{}

	void set(T* const ptr);
	T* get();

#if defined(ATOMIC_DWCAS_NEEDED)
	typename ABA_Wrapper<T*>::Counter get_counter()
	{
		return this->read_counter();
	}
#endif
};

template<typename T>
void ConcurrentPtr<T>::set(T* const ptr)
{
	// lamdas in function pointers can be inlined completely, but must not have captures
	bool (*inlined_modify_func)(T* const &, T*&, T* const) =	// implicit conversion
			[](T* const & /* ptr_cached */, T*& ptr_modify, T* const ptr_init) -> bool
			{
				ptr_modify = ptr_init;
				return true;
			};

	this->modify(inlined_modify_func, ptr);

/*
 *	compiles to:
 *
 *	0000000000000000 <_ZN7AsyncFW13ConcurrentPtrItE3setEPt>:
 *	   0:   53                      push   %rbx
 *	   1:   48 89 d8                mov    %rbx,%rax
 *	   4:   48 89 ca                mov    %rcx,%rdx
 *	   7:   f0 48 0f c7 0f          lock cmpxchg16b (%rdi)
 *	   c:   48 8d 4a 01             lea    0x1(%rdx),%rcx
 *	  10:   48 89 f3                mov    %rsi,%rbx
 *	  13:   f0 48 0f c7 0f          lock cmpxchg16b (%rdi)
 *	  18:   75 f2                   jne    c <_ZN7AsyncFW13ConcurrentPtrItE3setEPt+0xc>
 *	  1a:   5b                      pop    %rbx
 *	  1b:   c3                      retq
 */
}

template<typename T>
T* ConcurrentPtr<T>::get()
{
	return this->read();
}


} // namespace AsyncFW

#endif /* ATOMIC_CONCURRENT_POINTER_H_ */

