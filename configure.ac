AC_INIT([imageprocessor], [1.0])
AM_INIT_AUTOMAKE(1.1)
AC_CONFIG_MACRO_DIR(m4)
AC_CONFIG_SRCDIR([vibrance.c])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
#AC_CHECK_LIB(jpeg,  main, [], [
#   echo "Error - you need jpeg lib!"
#   exit -1
#])
AC_CHECK_LIB(jpeg,  main, [], [AC_MSG_ERROR("jpeg lib not found")])
AC_CHECK_LIB(exif,  main, [], [
   echo "Error - you need exif lib!"
   exit -1
])
AC_CHECK_LIB(m,  main, [], [
   echo "Error - you need math lib!"
   exit -1
])
AC_CHECK_LIB(avcodec,  main, [], [
   echo "Error - you need avcodec lib!"
   exit -1
])
AC_CHECK_LIB(avformat,  main, [], [
   echo "Error - you need avformat lib!"
   exit -1
])
AC_CHECK_LIB(avutil,  main, [], [
   echo "Error - you need avutil lib!"
   exit -1
])
AC_CHECK_LIB(pthread,  main, [], [
   echo "Error - you need pthread lib!"
   exit -1
])
PKG_CHECK_MODULES([OPENCV], [opencv4], [], [
   echo "Error - you need opencv4 lib!"
   exit -1
])


# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h stdlib.h string.h strings.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memset pow sqrt])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
