#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "module.h"

static int      item_timeout = 0;

extern void zrhai_module_init(void);
extern int zrhai_module_ping(void);

static int      dummy_ping(AGENT_REQUEST *request, AGENT_RESULT *result);


static ZBX_METRIC keys[] =
/*      KEY                     FLAG            FUNCTION        TEST PARAMETERS */
{
        {"dummy.ping",          0,              dummy_ping,     NULL},
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

static int      dummy_ping(AGENT_REQUEST *request, AGENT_RESULT *result)
{
        SET_UI64_RESULT(result, zrhai_module_ping());

        return SYSINFO_RET_OK;
}

/******************************************************************************
 *                                                                            *
 * Purpose: the function is called on agent startup                           *
 *          It should be used to call any initialization routines             *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - module initialization failed               *
 *                                                                            *
 * Comment: the module won't be loaded in case of ZBX_MODULE_FAIL             *
 *                                                                            *
 ******************************************************************************/
int     zbx_module_init(void)
{
        zrhai_module_init();

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
