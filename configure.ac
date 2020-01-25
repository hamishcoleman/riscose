AC_INIT([riscose], [0.01])
AC_CONFIG_SRCDIR([src/monty/monty.c])
AC_CONFIG_AUX_DIR(util)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

CFLAGS+="-Wall -Werror"

AC_PROG_CC
AC_PROG_YACC
AC_PROG_RANLIB
AC_PROG_AWK
AC_PATH_PROG(BINAWK, $AWK)
test -z "$BINAWK" && AC_MSG_ERROR([no acceptable $AWK found in \$PATH])
AC_PATH_PROG(PERL, perl)
test -z "$PERL" && AC_MSG_ERROR([no acceptable $ac_word found in \$PATH])

AC_DEFINE(CONF_DEBUGGING, 1, [enable -D command line debug option.])

AC_ARG_ENABLE(arm,
[  --enable-arm=ARM        Set the ARM emulation to use.  Choose one of
                          armul, sleeve, or native [[armul]]],
[case "$enableval" in
armul|sleeve|native) ARM=$enableval;;
*) AC_MSG_ERROR([bad value ${enableval} for --enable-arm])
esac],ARM=armul)
AC_SUBST(ARM)
AM_CONDITIONAL(ARM_ARMUL, test x$ARM = xarmul)
AM_CONDITIONAL(ARM_SLEEVE, test x$ARM = xsleeve)
AM_CONDITIONAL(ARM_NATIVE, test x$ARM = xnative)
if test x$ARM = xnative; then
    AC_DEFINE(NATIVE, 1, [synonym for ARM_NATIVE])
    AC_DEFINE(CONFIG_MEM_ONE2ONE, 1, [synonym for ARM_NATIVE])
fi

# FIXME: we should only use readline if --with-readline is used, or at
# least allow it to be disabled with --without-readline.
AC_CHECK_HEADERS(readline.h readline/readline.h)

AC_C_INLINE

AC_OUTPUT([ \
        Makefile \
        man/Makefile \
        src/Makefile \
        src/armul/Makefile \
        src/defmod/Makefile \
        src/include/Makefile \
        src/monty/Makefile \
        src/native/Makefile \
        src/os/Makefile \
        src/oslib/Makefile \
        src/osapi/Makefile \
        src/rom/Makefile \
        src/rom/compilerom \
        src/sleeve/Makefile \
        tests/Makefile \
        tests/armas \
        util/Makefile \
    ], \
    [chmod +x src/rom/compilerom tests/armas]
    )
