/*
 * atomic_dw_cas_wrapper.h
 *
 * (C) Simon Gleissner, simon@gleissner.de
 */

#ifndef ATOMIC_ABA_WRAPPER_H_
#define ATOMIC_ABA_WRAPPER_H_

#include <cstdint>
#include <type_traits>
#include <cstddef>

#include <asyncfw/atomic_dwcas_asm.h>
#include <asyncfw/atomic_llsc_asm.h>
#include <asyncfw/helper.h>


#if !defined(ATOMIC_DWCAS_NEEDED) && !defined(ATOMIC_LLSC_NEEDED)
#error "ABA solution strategy unknown"
#endif

#if !defined(PLATFORM_64) && !defined(PLATFORM_32)
#error "Platform unknown"
#endif


namespace AsyncFW
{

template <typename DATA>
class ABA_Wrapper
{
public:

#if defined(ATOMIC_DWCAS_NEEDED)
	static constexpr bool BASED_ON_DWCAS = 1;
	static constexpr bool BASED_ON_LLSC = 0;
#elif defined(ATOMIC_LLSC_NEEDED)
	static constexpr bool BASED_ON_DWCAS = 0;
	static constexpr bool BASED_ON_LLSC = 1;
#endif

#if defined(PLATFORM_64)
	static constexpr size_t MAX_DATA_SIZE = 8;
#elif defined(PLATFORM_32)
	static constexpr size_t MAX_DATA_SIZE = 4;
#endif

#if defined(PLATFORM_64)
	static constexpr size_t MEMBER_ALIGNMENT = sizeof(DATA)>4 ? 8 : 4;
#elif defined(PLATFORM_32)
	static constexpr size_t MEMBER_ALIGNMENT = 4;
#endif

#if defined(ATOMIC_DWCAS_NEEDED)
	static constexpr size_t ATOMIC_ALIGNMENT = MEMBER_ALIGNMENT*2;
#elif defined(ATOMIC_LLSC_NEEDED)
	static constexpr size_t ATOMIC_ALIGNMENT = MEMBER_ALIGNMENT;
#endif

#if defined(ATOMIC_DWCAS_NEEDED)
#if defined(PLATFORM_64)
	using Atomic_DWCAS = typename std::conditional<(sizeof(DATA)>4), uint64_t, uint32_t>::type;
#elif defined(PLATFORM_32)
	using Atomic_DWCAS = uint32_t;
#endif
	using Counter = Atomic_DWCAS;
#elif defined(ATOMIC_LLSC_NEEDED)
#if defined(PLATFORM_64)
	using Atomic_LLSC = typename std::conditional<(sizeof(DATA)>4), uint64_t, uint32_t>::type;
#elif defined(PLATFORM_32)
	using Atomic_LLSC = uint32_t;
#endif
#endif

	static_assert(std::is_pointer_v<DATA> || std::is_integral_v<DATA>, "DATA is neither pointer, nor integral type.");
	static_assert(sizeof(DATA) <= MAX_DATA_SIZE, "DATA size wrong.");

private:

	union alignas(ATOMIC_ALIGNMENT) DataWrapper
	{
#if defined(ATOMIC_DWCAS_NEEDED)

		struct
		{
			// TODO: maybe store 'data' in union with integer value for alignment
			DATA data;	// aligned to ATOMIC_ALIGNMENT
			struct alignas(MEMBER_ALIGNMENT)
			{
				Counter counter;
			};
		};
		Atomic_DWCAS dw_atomic[2];

		DataWrapper() = default;	// union is per default uninitialized
		DataWrapper(DATA init)
		: data(init), counter(1) {}

#elif defined(ATOMIC_LLSC_NEEDED)

		DATA data;	// aligned to ATOMIC_ALIGNMENT
		Atomic_LLSC atomic;

		DataWrapper() = default;	// union is per default uninitialized
		DataWrapper(DATA init) : data(init) {}

#endif
	};

	DataWrapper wrapper;

protected:
	ABA_Wrapper(DATA init)
	{
		DataWrapper desired(init);
		atomic_dw_store(wrapper.dw_atomic, desired.dw_atomic);
	}


	// TODO: atomic
	DATA read()
	{
		DataWrapper cache;
		atomic_dw_load(wrapper.dw_atomic, cache.dw_atomic);
		return cache.data;
	}

#if defined(ATOMIC_DWCAS_NEEDED)
	Counter read_counter()
	{
		DataWrapper cache;
		atomic_dw_load(wrapper.dw_atomic, cache.dw_atomic);
		return cache.counter;
	}
#endif

	template<typename... ARGS>
	inline bool modify [[gnu::always_inline, gnu::optimize("align-loops=1")]] (
			bool (*modifier_func)(const DATA&, DATA&, ARGS...),
			ARGS... args)
	{
#if defined(ATOMIC_DWCAS_NEEDED)

		DataWrapper cache;
		DataWrapper desired;
		bool success;

		atomic_dw_load(wrapper.dw_atomic, cache.dw_atomic);
		do
		{
			success = modifier_func(cache.data, desired.data, args...);
			if(success == false)
				break;
			desired.counter = cache.counter + 1;
		}
		while(UNLIKELY(atomic_dw_cas(wrapper.dw_atomic, cache.dw_atomic, desired.dw_atomic)==false));

		return success;

#elif defined(ATOMIC_LLSC_NEEDED)

	typename ABA_Wrapper<DATA>::DataWrapper desired;
	bool success;

	do
	{
		typename ABA_Wrapper<DATA>::DataWrapper cached;

		load_link(&aba_ptr->wrapper.atomic, &cached.atomic);
		success = modifier_func(cached.data, desired.data, args...);
		if(success == false)
			break;
	}
	while(store_conditional(&aba_ptr->wrapper.atomic, &desired.atomic)==false);

	return success;

#endif
	}
};

} // namespace AsyncFW

#endif /* ATOMIC_ABA_WRAPPER_H_ */
