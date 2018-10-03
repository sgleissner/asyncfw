
# asyncfwincludedir = $(includedir)/asyncfw

# install headers into /usr/include/<pkgname>/
pkginclude_HEADERS = \
	src/asyncfw/generated_config.h			\
	src/asyncfw/helper.h					\
	src/asyncfw/atomic_concurrent_pointer.h	\
	src/asyncfw/atomic_aba_wrapper.h		\
	src/asyncfw/atomic_dwcas_asm.h			\
	src/asyncfw/atomic_llsc_asm.h
	
#	src/asyncfw/asyncfw.h			\
#	src/asyncfw/version.h			\
#	src/asyncfw/atomic.h			\
#	src/asyncfw/futex.h				\
#	src/asyncfw/eventbuffer.h		\
#	src/asyncfw/handler.h			\
#	src/asyncfw/redirector.h		\
#	src/asyncfw/descriptor.h		\
#	src/asyncfw/timer.h				\
#	src/asyncfw/poller.h

bin_PROGRAMS = test_asyncfw

# -fPIC

OPTIONS_ARM_32=
OPTIONS_ARM_64=
OPTIONS_X86_32=
OPTIONS_X86_64=-march=nehalem -mcx16

OPTIONS_CPU=$(OPTIONS_X86_64)

AM_CPPFLAGS=-I src
AM_CXXFLAGS=-std=c++17 -Wall -Wextra -O3 $(OPTIONS_CPU)
AM_LDFLAGS=

test_asyncfw_SOURCES =	\
	src/test/test_aba.cpp

#	src/tests.cpp		\
#	src/futex.cpp		\
#	src/eventbuffer.cpp	\
#	src/handler.cpp		\
#	src/redirector.cpp	\
#	src/descriptor.cpp	\
#	src/timer.cpp		\
#	src/poller.cpp
