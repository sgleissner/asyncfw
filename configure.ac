# configure autoconf & automake
AC_INIT([asyncfw], [0.0.1], [bugs@asyncfw.de], [asyncfw], [http://asyncfw.de])
AC_CONFIG_SRCDIR([config.h.in])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# custom config header, usable in library headers
AC_CONFIG_HEADERS(config.h)
AX_PREFIX_CONFIG_H(src/asyncfw/generated_config.h)

# use C++17
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX(17, noext, mandatory)

# generate Makefile 
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
