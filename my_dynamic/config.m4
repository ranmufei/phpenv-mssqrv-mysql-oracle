dnl $Id$
dnl config.m4 for extension my_dynamic

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(my_dynamic, for my_dynamic support,
dnl Make sure that the comment is aligned:
[  --with-my_dynamic             Include my_dynamic support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(my_dynamic, whether to enable my_dynamic support,
dnl Make sure that the comment is aligned:
dnl [  --enable-my_dynamic           Enable my_dynamic support])

if test "$PHP_MY_DYNAMIC" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-my_dynamic -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/my_dynamic.h"  # you most likely want to change this
  dnl if test -r $PHP_MY_DYNAMIC/$SEARCH_FOR; then # path given as parameter
  dnl   MY_DYNAMIC_DIR=$PHP_MY_DYNAMIC
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for my_dynamic files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MY_DYNAMIC_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MY_DYNAMIC_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the my_dynamic distribution])
  dnl fi

  dnl # --with-my_dynamic -> add include path
  dnl PHP_ADD_INCLUDE($MY_DYNAMIC_DIR/include)

  dnl # --with-my_dynamic -> check for lib and symbol presence
  dnl LIBNAME=my_dynamic # you may want to change this
  dnl LIBSYMBOL=my_dynamic # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MY_DYNAMIC_DIR/$PHP_LIBDIR, MY_DYNAMIC_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MY_DYNAMICLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong my_dynamic lib version or lib not found])
  dnl ],[
  dnl   -L$MY_DYNAMIC_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  PHP_SUBST(MY_DYNAMIC_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(hcnetsdk,/usr/lib,MY_DYNAMIC_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(HCCore,/usr/lib,MY_DYNAMIC_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(HCPlayBack,/usr/lib,MY_DYNAMIC_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(HCCoreDevCfg,/usr/lib,MY_DYNAMIC_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(HCGeneralCfgMgr,/usr/lib,MY_DYNAMIC_SHARED_LIBADD)
  dnl PHP_ADD_LIBRARY_WITH_PATH(PlayCtrl,/usr/lib,MY_DYNAMIC_SHARED_LIBADD)
  PHP_SUBST(MY_DYNAMIC_SHARED_LIBADD)
  PHP_NEW_EXTENSION(my_dynamic, my_dynamic.c, $ext_shared)
fi
