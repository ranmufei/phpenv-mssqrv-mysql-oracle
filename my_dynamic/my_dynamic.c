/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdbool.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_my_dynamic.h"
#include "HCNetSDK.h"

/* If you declare any globals in php_my_dynamic.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(my_dynamic)
*/
/* True global resources - no need for thread safety here */
static int le_my_dynamic;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("my_dynamic.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_my_dynamic_globals, my_dynamic_globals)
    STD_PHP_INI_ENTRY("my_dynamic.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_my_dynamic_globals, my_dynamic_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_my_dynamic_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_my_dynamic_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "my_dynamic", arg);
	RETURN_STRINGL(strg, len);
}
//初始化注册设备
PHP_FUNCTION(hk_login)
{
  char *str,*sDeviceAddress,*sUserName,*sPassword;
  int  str_len,sDeviceAddress_len,sUserName_len,sPassword_len;
  WORD wPort=8000;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssss|l",&str,&str_len,&sDeviceAddress,&sDeviceAddress_len,&sUserName,&sUserName_len,&sPassword,&sPassword_len,&wPort) == FAILURE) {
    RETURN_STRING("parmas error");
  } 
  //初始�
  bool result = NET_DVR_Init();
  //设置连接时间与重连时间
  NET_DVR_SetConnectTime(2000, 1);
  NET_DVR_SetReconnect(10000, true);
  // 注册设备
  long lUserID;
  //登录参数，包括设备地址、登录用户、密码等
  NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
  struLoginInfo.bUseAsynLogin = 0; //同步登录方
  strcpy(struLoginInfo.sDeviceAddress,sDeviceAddress); //设备IP地址
  struLoginInfo.wPort = wPort; //设备服务�
  strcpy(struLoginInfo.sUserName,sUserName); //设备登录用户名
  strcpy(struLoginInfo.sPassword,sPassword); //设备登录密码 
  //设备信息, 输出�
  NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
  lUserID = NET_DVR_Login_V40(&struLoginInfo,&struDeviceInfoV40);
  if (lUserID < 0)
  {
    char msg[20];
    sprintf(msg, "login error %d\n",NET_DVR_GetLastError());
    NET_DVR_Cleanup();
    RETURN_STRING(msg);
  }
  RETURN_LONG(lUserID);
}

PHP_FUNCTION(hk_login_out){
 long lUserID;
 if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",&lUserID) == FAILURE) {
    NET_DVR_Cleanup();
    RETURN_STRING("parmas error");
 }
 NET_DVR_Logout(lUserID);
 NET_DVR_Cleanup();
 RETURN_LONG(1);
}

PHP_FUNCTION(hk_get_time){
  long lUserID;
  DWORD dwReturnLen;
  if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",&lUserID) == FAILURE) {
    RETURN_LONG(0);
 }
  NET_DVR_TIME struParams = { 0 };
  if(!NET_DVR_GetDVRConfig(lUserID,NET_DVR_GET_TIMECFG,-1,&struParams, sizeof(NET_DVR_TIME), &dwReturnLen)){
    RETURN_LONG(NET_DVR_GetLastError());
  }else{
    char msg[20];
    sprintf(msg,"%d-%d-%d %d:%d:%d\n", struParams.dwYear, struParams.dwMonth, struParams.dwDay, struParams.dwHour, struParams.dwMinute, struParams.dwSecond);
    RETURN_STRING(msg);
  }
}

PHP_FUNCTION(hk_set_time){
  long lUserID;
  WORD  year,month,day,hour,minute,second;
  if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllllll",&lUserID,&year,&month,&day,&hour,&minute,&second) == FAILURE) {
    RETURN_STRING("parmas error");
  }
  NET_DVR_TIME struParams = { 0 };
  struParams.dwYear     = year;
  struParams.dwMonth    = month;
  struParams.dwDay      = day;
  struParams.dwHour     = hour;
  struParams.dwMinute   = minute;
  struParams.dwSecond   = second;
  if(!NET_DVR_SetDVRConfig(lUserID,NET_DVR_SET_TIMECFG,-1,&struParams, sizeof(NET_DVR_TIME))){
    char msg[20];
    sprintf(msg, "set time error %d\n",NET_DVR_GetLastError());
    RETURN_STRING(msg);
  }
  RETURN_LONG(1);
}

PHP_FUNCTION(hk_down)
{
  long lUserID;
  char *path;
  WORD channel_num,begin_year,begin_month,begin_day,begin_hour,begin_minute,begin_second,end_year,end_month,end_day,end_hour,end_minute,end_second,path_len;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllllllllllllls",&lUserID,&channel_num,&begin_year,&begin_month,&begin_day,&begin_hour,&begin_minute,&begin_second,&end_year,&end_month,&end_day,&end_hour,&end_minute,&end_second,&path,&path_len) == FAILURE) {
    RETURN_STRING("parmas error");
  }
  NET_DVR_PLAYCOND struDownloadCond={0};
  struDownloadCond.dwChannel                = channel_num;
  struDownloadCond.struStartTime.dwYear     = begin_year;
  struDownloadCond.struStartTime.dwMonth    = begin_month;
  struDownloadCond.struStartTime.dwDay      = begin_day;
  struDownloadCond.struStartTime.dwHour     = begin_hour;
  struDownloadCond.struStartTime.dwMinute   = begin_minute;
  struDownloadCond.struStartTime.dwSecond   = begin_second;
  struDownloadCond.struStopTime.dwYear      = end_year;
  struDownloadCond.struStopTime.dwMonth     = end_month;
  struDownloadCond.struStopTime.dwDay       = end_day;
  struDownloadCond.struStopTime.dwHour      = end_hour;
  struDownloadCond.struStopTime.dwMinute    = end_minute;
  struDownloadCond.struStopTime.dwSecond    = end_second;
  struDownloadCond.byStreamType             = 1;
 	//按时间下载
	int hPlayback;
	hPlayback = NET_DVR_GetFileByTime_V40(lUserID, path,&struDownloadCond); 
	if(hPlayback < 0)
	{
	  char msg[20];
          sprintf(msg, "down error %d\n",NET_DVR_GetLastError());
    	  RETURN_STRING(msg);
	}
	//---------------------------------------
  //开始下载
  if(!NET_DVR_PlayBackControl_V40(hPlayback, NET_DVR_PLAYSTART, NULL, 0, NULL,NULL))
  {
    char msg[20];
    sprintf(msg, "palyback error %d\n",NET_DVR_GetLastError());
    RETURN_STRING(msg);
  }

  int nPos = 0;
  for(nPos = 0; nPos < 100&&nPos>=0; nPos = NET_DVR_GetDownloadPos(hPlayback))
  {
    sleep(1);  //millisecond
  }
  if(!NET_DVR_StopGetFile(hPlayback))
  {
    char msg[20];
    sprintf(msg, "stop down error %d\n",NET_DVR_GetLastError());
    RETURN_STRING(msg);

  }
  if(nPos<0||nPos>100)
  {
    char msg[20];
    sprintf(msg, "downing error %d\n",NET_DVR_GetLastError());
    RETURN_STRING(msg);
  }
  RETURN_LONG(1);
}



/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_my_dynamic_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_my_dynamic_init_globals(zend_my_dynamic_globals *my_dynamic_globals)
{
	my_dynamic_globals->global_value = 0;
	my_dynamic_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(my_dynamic)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(my_dynamic)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(my_dynamic)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(my_dynamic)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(my_dynamic)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "my_dynamic support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ my_dynamic_functions[]
 *
 * Every user visible function must have an entry in my_dynamic_functions[].
 */
const zend_function_entry my_dynamic_functions[] = {
	PHP_FE(hk_down,NULL)
        PHP_FE(hk_login,NULL)
        PHP_FE(hk_login_out,NULL)
        PHP_FE(hk_get_time,NULL)
        PHP_FE(hk_set_time,NULL)
	PHP_FE(confirm_my_dynamic_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in my_dynamic_functions[] */
};
/* }}} */

/* {{{ my_dynamic_module_entry
 */
zend_module_entry my_dynamic_module_entry = {
	STANDARD_MODULE_HEADER,
	"my_dynamic",
	my_dynamic_functions,
	PHP_MINIT(my_dynamic),
	PHP_MSHUTDOWN(my_dynamic),
	PHP_RINIT(my_dynamic),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(my_dynamic),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(my_dynamic),
	PHP_MY_DYNAMIC_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MY_DYNAMIC
ZEND_GET_MODULE(my_dynamic)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
