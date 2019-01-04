
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dh_nvr.h"
#include "dhnetsdk.h"

/* If you declare any globals in php_dh_nvr.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(dh_nvr)
*/

/* True global resources - no need for thread safety here */
static int le_dh_nvr;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("dh_nvr.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_dh_nvr_globals, dh_nvr_globals)
    STD_PHP_INI_ENTRY("dh_nvr.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_dh_nvr_globals, dh_nvr_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_dh_nvr_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_dh_nvr_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "dh_nvr", arg);
	RETURN_STRINGL(strg, len);
}

PHP_FUNCTION(dh_login)
{
  	char *str,*sDeviceAddress,*sUserName,*sPassword;
  	int  str_len,sDeviceAddress_len,sUserName_len,sPassword_len;
  	int  wPort=37777;
  	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssss|l",&str,&str_len,&sDeviceAddress,&sDeviceAddress_len,&sUserName,&	sUserName_len,&sPassword,&sPassword_len,&wPort) == FAILURE) {
    		RETURN_STRING("parmas error");
  	}
  	//初始化
  	BOOL result = CLIENT_Init(NULL, 0); 

  	char szDevIp[64] = {0};
  	strcpy(szDevIp,sDeviceAddress);
	NET_DEVICEINFO stDevInfo = {0};
	int nError = 0;
	LLONG lLoginHandle = 0;
	LLONG hPlayBack = 0;
	lLoginHandle = CLIENT_Login(szDevIp, wPort,sUserName,sPassword, &stDevInfo, &nError);
	if (lLoginHandle == 0)
	{
                char msg[20];
    		sprintf(msg, "login error %d\n",nError);
		CLIENT_Cleanup();
	    	RETURN_STRING(msg);
	}
        RETURN_LONG(lLoginHandle);
}

PHP_FUNCTION(dh_login_out){
        long lUserID;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",&lUserID) == FAILURE) {
        	RETURN_STRING("parmas error");
        }
        CLIENT_Logout(lUserID);
        CLIENT_Cleanup();
        RETURN_LONG(1);
}

PHP_FUNCTION(dh_get_time){
  	long lUserID;
  	DWORD dwReturnLen=0;
  	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",&lUserID) == FAILURE) {
    		RETURN_LONG(0);
 	}
  	NET_TIME struParams = { 0 };
  	if(!CLIENT_GetDevConfig(lUserID,DH_DEV_TIMECFG,-1,&struParams, sizeof(NET_TIME), &dwReturnLen,500)){
    		RETURN_LONG(CLIENT_GetLastError()&(0x7fffffff));
  	}else{
    		char msg[20];
    		sprintf(msg,"%d-%d-%d %d:%d:%d\n", struParams.dwYear, struParams.dwMonth, struParams.dwDay, struParams.dwHour, struParams.dwMinute, struParams.dwSecond);
    		RETURN_STRING(msg);
	}
}

PHP_FUNCTION(dh_set_time){
  	long lUserID;
  	WORD  year,month,day,hour,minute,second;
  	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllllll",&lUserID,&year,&month,&day,&hour,&minute,&second) == FAILURE) {
    		RETURN_STRING("parmas error");
  	}	
  	NET_TIME struParams = { 0 };
  	struParams.dwYear     = year;
  	struParams.dwMonth    = month;
  	struParams.dwDay      = day;
  	struParams.dwHour     = hour;
  	struParams.dwMinute   = minute;
  	struParams.dwSecond   = second;
  	if(!CLIENT_SetDevConfig(lUserID,DH_DEV_TIMECFG,-1,&struParams, sizeof(NET_TIME),500)){
		char msg[20];
    		sprintf(msg, "set time error %d\n",CLIENT_GetLastError()&(0x7fffffff));
    		RETURN_STRING(msg);
  	}
  	RETURN_LONG(1);
}

PHP_FUNCTION(dh_down)
{
  	long lUserID;
  	char *path;
  	WORD channel_num,begin_year,begin_month,begin_day,begin_hour,begin_minute,begin_second,end_year,end_month,end_day,end_hour,end_minute,end_second,path_len;
  	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllllllllllllls",&lUserID,&channel_num,&begin_year,&begin_month,&begin_day,&begin_hour,&begin_minute,&begin_second,&end_year,&end_month,&end_day,&end_hour,&end_minute,&end_second,&path,&path_len) == FAILURE)         {  
    		RETURN_STRING("parmas error");
  	}
        LLONG hdownloadfile = 0;
  	NET_TIME StartTime = {0};
	NET_TIME StopTime = {0};
	StartTime.dwYear = begin_year;
	StartTime.dwMonth = begin_month;
	StartTime.dwDay = begin_day;
	StartTime.dwHour = begin_hour;
	StartTime.dwMinute = begin_minute;
	StartTime.dwSecond = begin_second;
	StopTime.dwYear = end_year;
	StopTime.dwMonth = end_month;
	StopTime.dwDay = end_day;
	StopTime.dwHour = end_hour;
	StopTime.dwMinute = end_minute;
	StopTime.dwSecond = end_second;
	hdownloadfile = CLIENT_DownloadByTime(lUserID, channel_num, 0,&StartTime, &StopTime,path,NULL,(DWORD)0);
 	//按时间下载 
	if (hdownloadfile != 0)
	{
		CLIENT_StopDownload(hdownloadfile);
	}else{
                char msg[20];
                sprintf(msg, "down error %d\n",CLIENT_GetLastError()&(0x7fffffff));
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


/* {{{ php_dh_nvr_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_dh_nvr_init_globals(zend_dh_nvr_globals *dh_nvr_globals)
{
	dh_nvr_globals->global_value = 0;
	dh_nvr_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(dh_nvr)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(dh_nvr)
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
PHP_RINIT_FUNCTION(dh_nvr)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(dh_nvr)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(dh_nvr)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "dh_nvr support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ dh_nvr_functions[]
 *
 * Every user visible function must have an entry in dh_nvr_functions[].
 */
const zend_function_entry dh_nvr_functions[] = {
	PHP_FE(dh_down,NULL)
  	PHP_FE(dh_login,NULL)
        PHP_FE(dh_login_out,NULL)
        PHP_FE(dh_get_time,NULL)
        PHP_FE(dh_set_time,NULL)
	PHP_FE(confirm_dh_nvr_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in dh_nvr_functions[] */
};
/* }}} */

/* {{{ dh_nvr_module_entry
 */
zend_module_entry dh_nvr_module_entry = {
	STANDARD_MODULE_HEADER,
	"dh_nvr",
	dh_nvr_functions,
	PHP_MINIT(dh_nvr),
	PHP_MSHUTDOWN(dh_nvr),
	PHP_RINIT(dh_nvr),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(dh_nvr),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(dh_nvr),
	PHP_DH_NVR_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DH_NVR
ZEND_GET_MODULE(dh_nvr)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
