/*
 * appender_type_stderr.c
 *
 * Copyright 2020, Kevin L. Esteb (kevin@kesteb.us). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/appender.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************/
static int stderr_open(log4c_appender_t* this) {

    FILE* fp = log4c_appender_get_udata(this);
    
    if (fp)
        return 0;
    
	fp = stderr;
    
    /* unbuffered mode */
    setbuf(fp, NULL);
    
    log4c_appender_set_udata(this, fp);

    return 0;

}

/*******************************************************************************/
static int stderr_append(log4c_appender_t* this, 
			 const log4c_logging_event_t* a_event)
{
    FILE* fp = log4c_appender_get_udata(this);
    
    return fprintf(fp, "%s\n", a_event->evt_rendered_msg);
}

/*******************************************************************************/
static int stderr_close(log4c_appender_t* this)
{
    int retval = 0;
    FILE *fp = log4c_appender_get_udata(this);

    if (!fp || fp == stderr)
        return 0;

    return retval;
}

/*******************************************************************************/
const log4c_appender_type_t log4c_appender_type_stderr = {
    "stderr",
    stderr_open,
    stderr_append,
    stderr_close,
    NULL
};

