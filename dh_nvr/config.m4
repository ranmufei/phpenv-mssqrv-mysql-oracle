dnl $Id$
dnl config.m4 for extension dh_nvr

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(dh_nvr, for dh_nvr support,
dnl Make sure that the comment is aligned:
[  --with-dh_nvr             Include dh_nvr support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(dh_nvr, whether to enable dh_nvr support,
dnl Make sure that the comment is aligned:
dnl [  --enable-dh_nvr           Enable dh_nvr support])

if test "$PHP_DH_NVR" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-dh_nvr -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/dh_nvr.h"  # you most likely want to change this
  dnl if test -r $PHP_DH_NVR/$SEARCH_FOR; then # path given as parameter
  dnl   DH_NVR_DIR=$PHP_DH_NVR
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for dh_nvr files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DH_NVR_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DH_NVR_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the dh_nvr distribution])
  dnl fi

  dnl # --with-dh_nvr -> add include path
  dnl PHP_ADD_INCLUDE($DH_NVR_DIR/include)

  dnl # --with-dh_nvr -> check for lib and symbol presence
  dnl LIBNAME=dh_nvr # you may want to change this
  dnl LIBSYMBOL=dh_nvr # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DH_NVR_DIR/$PHP_LIBDIR, DH_NVR_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DH_NVRLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong dh_nvr lib version or lib not found])
  dnl ],[
  dnl   -L$DH_NVR_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  PHP_SUBST(DH_NVR_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(dhnetsdk,/usr/lib,DH_NVR_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(avnetsdk,/usr/lib,DH_NVR_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(dhconfigsdk,/usr/lib,DH_NVR_SHARED_LIBADD)
  PHP_NEW_EXTENSION(dh_nvr, dh_nvr.c, $ext_shared)
fi
