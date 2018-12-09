AC_INIT([imageprocessor], [1.0])
AM_INIT_AUTOMAKE(1.1)
AC_CONFIG_MACRO_DIR(m4)
AC_CONFIG_SRCDIR([vibrance.c])
AC_PROG_CC
AC_PROG_CXX
AC_CHECK_LIB(jpeg,  main, [], [
   echo "Error - you need jpeg lib!"
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
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
