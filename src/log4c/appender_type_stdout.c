/*
 * appender_type_stdout.c
 *
 * Copyright 2020, Kevin L. Esteb (kevin@kesteb.us). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/appender.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************/
static int stdout_open(log4c_appender_t* this) {

    FILE* fp = log4c_appender_get_udata(this);
    
    if (fp)
        return 0;
    
	fp = stdout;
    
    /* unbuffered mode */
    setbuf(fp, NULL);
    
    log4c_appender_set_udata(this, fp);

    return 0;

}

/*******************************************************************************/
static int stdout_append(log4c_appender_t* this, 
			 const log4c_logging_event_t* a_event)
{
    FILE* fp = log4c_appender_get_udata(this);
    
    return fprintf(fp, "%s\n", a_event->evt_rendered_msg);
}

/*******************************************************************************/
static int stdout_close(log4c_appender_t* this)
{
    int retval = 0;
    FILE *fp = log4c_appender_get_udata(this);

    if (!fp || fp == stdout)
        return 0;

    return retval;
}

/*******************************************************************************/
const log4c_appender_type_t log4c_appender_type_stdout = {
    "stdout",
    stdout_open,
    stdout_append,
    stdout_close,
    NULL
};

