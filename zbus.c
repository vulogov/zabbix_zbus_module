#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "module.h"

static int      item_timeout = 0;

extern int zbus_module_init(void);
extern int zbus_module_ping(void);
extern char* zbus_module_get_str(char*);
extern char* zbus_module_query_str(char*, char*);
extern char* zbus_module_get_raw(char*);
extern void free_string(char*);

static int      zbus_ping(AGENT_REQUEST *request, AGENT_RESULT *result);
static int      zbus_get_str(AGENT_REQUEST *request, AGENT_RESULT *result);
static int      zbus_get_int(AGENT_REQUEST *request, AGENT_RESULT *result);
static int      zbus_get_float(AGENT_REQUEST *request, AGENT_RESULT *result);
static int      zbus_get_raw(AGENT_REQUEST *request, AGENT_RESULT *result);
static int      zbus_query_raw(AGENT_REQUEST *request, AGENT_RESULT *result);
static int      zbus_query_int(AGENT_REQUEST *request, AGENT_RESULT *result);
static int      zbus_query_float(AGENT_REQUEST *request, AGENT_RESULT *result);


static ZBX_METRIC keys[] =
/*      KEY                     FLAG            FUNCTION        TEST PARAMETERS */
{
        {"zbus.ping",          0,               zbus_ping,       NULL},
        {"zbus.get_str",       CF_HAVEPARAMS,   zbus_get_str,    NULL},
        {"zbus.get_int",       CF_HAVEPARAMS,   zbus_get_int,    NULL},
        {"zbus.get_float",     CF_HAVEPARAMS,   zbus_get_float,  NULL},
        {"zbus.get",           CF_HAVEPARAMS,   zbus_get_raw,    NULL},
        {"zbus.query_str",     CF_HAVEPARAMS,   zbus_query_raw,    NULL},
        {"zbus.query_int",     CF_HAVEPARAMS,   zbus_query_int,    NULL},
        {"zbus.query_float",   CF_HAVEPARAMS,   zbus_query_float,  NULL},
        {"zbus.query",         CF_HAVEPARAMS,   zbus_query_raw,    NULL},
        {NULL}
};

/******************************************************************************
 *                                                                            *
 * Purpose: returns version number of the module interface                    *
 *                                                                            *
 * Return value: ZBX_MODULE_API_VERSION - version of module.h module is       *
 *               compiled with, in order to load module successfully Zabbix   *
 *               MUST be compiled with the same version of this header file   *
 *                                                                            *
 ******************************************************************************/
int     zbx_module_api_version(void)
{
        return ZBX_MODULE_API_VERSION;
}

/******************************************************************************
 *                                                                            *
 * Purpose: set timeout value for processing of items                         *
 *                                                                            *
 * Parameters: timeout - timeout in seconds, 0 - no timeout set               *
 *                                                                            *
 ******************************************************************************/
void    zbx_module_item_timeout(int timeout)
{
        item_timeout = timeout;
}

/******************************************************************************
 *                                                                            *
 * Purpose: returns list of item keys supported by the module                 *
 *                                                                            *
 * Return value: list of item keys                                            *
 *                                                                            *
 ******************************************************************************/
ZBX_METRIC      *zbx_module_item_list(void)
{
        return keys;
}

static int      zbus_ping(AGENT_REQUEST *request, AGENT_RESULT *result)
{
        SET_UI64_RESULT(result, zbus_module_ping());

        return SYSINFO_RET_OK;
}

static int      zbus_get_str(AGENT_REQUEST *request, AGENT_RESULT *result)
{

    if (request->nparam == 0) {
      SET_MSG_RESULT(result, strdup("Key not provided"));
      return SYSINFO_RET_FAIL;
    } else {
      char *buffer = zbus_module_get_str(get_rparam(request, 0));

      if (buffer == NULL) {
        SET_MSG_RESULT(result, strdup("Key not found on zbus"));
        return SYSINFO_RET_FAIL;
      } else {
        SET_STR_RESULT(result, strdup(buffer));
        free_string(buffer);
        return SYSINFO_RET_OK;
      }
    }
}

static int      zbus_get_raw(AGENT_REQUEST *request, AGENT_RESULT *result)
{

    if (request->nparam == 0) {
      SET_MSG_RESULT(result, strdup("Key not provided"));
      return SYSINFO_RET_FAIL;
    } else {
      char *buffer = zbus_module_get_raw(get_rparam(request, 0));

      if (buffer == NULL) {
        SET_MSG_RESULT(result, strdup("Key not found on zbus"));
        return SYSINFO_RET_FAIL;
      } else {
        SET_STR_RESULT(result, strdup(buffer));
        free_string(buffer);
        return SYSINFO_RET_OK;
      }
    }
}

static int      zbus_query_raw(AGENT_REQUEST *request, AGENT_RESULT *result)
{

    if (request->nparam < 2) {
      SET_MSG_RESULT(result, strdup("Key and query not provided"));
      return SYSINFO_RET_FAIL;
    } else {
      char *buffer = zbus_module_query_str(get_rparam(request, 0), get_rparam(request, 1));

      if (buffer == NULL) {
        SET_MSG_RESULT(result, strdup("Key not found on zbus, or query returned NONE"));
        return SYSINFO_RET_FAIL;
      } else {
        SET_STR_RESULT(result, strdup(buffer));
        free_string(buffer);
        return SYSINFO_RET_OK;
      }
    }
}


static int      zbus_get_int(AGENT_REQUEST *request, AGENT_RESULT *result)
{

    if (request->nparam == 0) {
      SET_MSG_RESULT(result, strdup("Key not provided"));
      return SYSINFO_RET_FAIL;
    } else {
      char *buffer = zbus_module_get_str(get_rparam(request, 0));

      if (buffer == NULL) {
        SET_MSG_RESULT(result, strdup("Key not found on zbus"));
        return SYSINFO_RET_FAIL;
      } else {
        char* ptr;
        long res = strtol(buffer, &ptr, 10);
        if (buffer == ptr) {
          free_string(buffer);
          SET_MSG_RESULT(result, strdup("Error parsing value"));
          return SYSINFO_RET_FAIL;
        }
        SET_UI64_RESULT(result, res);
        free_string(buffer);
        return SYSINFO_RET_OK;
      }
    }
}

static int      zbus_query_int(AGENT_REQUEST *request, AGENT_RESULT *result)
{

    if (request->nparam < 2) {
      SET_MSG_RESULT(result, strdup("Key and query not provided"));
      return SYSINFO_RET_FAIL;
    } else {
      char *buffer = zbus_module_query_str(get_rparam(request, 0), get_rparam(request, 1));

      if (buffer == NULL) {
        SET_MSG_RESULT(result, strdup("Key not found on zbus, or query returned NONE"));
        return SYSINFO_RET_FAIL;
      } else {
        char* ptr;
        long res = strtol(buffer, &ptr, 10);
        if (buffer == ptr) {
          free_string(buffer);
          SET_MSG_RESULT(result, strdup("Error parsing value"));
          return SYSINFO_RET_FAIL;
        }
        SET_UI64_RESULT(result, res);
        free_string(buffer);
        return SYSINFO_RET_OK;
      }
    }
}

static int      zbus_get_float(AGENT_REQUEST *request, AGENT_RESULT *result)
{

    if (request->nparam == 0) {
      SET_MSG_RESULT(result, strdup("Key not provided"));
      return SYSINFO_RET_FAIL;
    } else {
      char *buffer = zbus_module_get_str(get_rparam(request, 0));

      if (buffer == NULL) {
        SET_MSG_RESULT(result, strdup("Key not found on zbus"));
        return SYSINFO_RET_FAIL;
      } else {
        char* ptr;
        double res = strtod(buffer, &ptr);
        if (buffer == ptr) {
          free_string(buffer);
          SET_MSG_RESULT(result, strdup("Error parsing value"));
          return SYSINFO_RET_FAIL;
        }
        SET_DBL_RESULT(result, res);
        free_string(buffer);
        return SYSINFO_RET_OK;
      }
    }
}

static int      zbus_query_float(AGENT_REQUEST *request, AGENT_RESULT *result)
{

    if (request->nparam < 2) {
      SET_MSG_RESULT(result, strdup("Key and query not provided"));
      return SYSINFO_RET_FAIL;
    } else {
      char *buffer = zbus_module_query_str(get_rparam(request, 0), get_rparam(request, 1));

      if (buffer == NULL) {
        SET_MSG_RESULT(result, strdup("Key not found on zbus, or query returned NONE"));
        return SYSINFO_RET_FAIL;
      } else {
        char* ptr;
        double res = strtod(buffer, &ptr);
        if (buffer == ptr) {
          free_string(buffer);
          SET_MSG_RESULT(result, strdup("Error parsing value"));
          return SYSINFO_RET_FAIL;
        }
        SET_DBL_RESULT(result, res);
        free_string(buffer);
        return SYSINFO_RET_OK;
      }
    }
}

int     zbx_module_init(void)
{
        if (zbus_module_init() == 0) {
            return ZBX_MODULE_FAIL;
        }
        return ZBX_MODULE_OK;
}

/******************************************************************************
 *                                                                            *
 * Purpose: the function is called on agent shutdown                          *
 *          It should be used to cleanup used resources if there are any      *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - function failed                            *
 *                                                                            *
 ******************************************************************************/
int     zbx_module_uninit(void)
{
        return ZBX_MODULE_OK;
}
