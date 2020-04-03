/* 
 * appender_type_stdout.h
 * 
 * Copyright 2020, Kevin L. Esteb (kevin@kesteb.us). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef log4c_appender_type_stdout_h
#define log4c_appender_type_stdout_h

/**
 * @file appender_type_stdout.h
 *
 * @brief Log4c stream appender interface.
 *
 * The stdout appender uses @c stdout for logging. 
 *
 **/

#include <log4c/defs.h>
#include <log4c/appender.h>

__LOG4C_BEGIN_DECLS

/**
 * Stdout appender type definition.
 *
 * This should be used as a parameter to the log4c_appender_set_type()
 * routine to set the type of the appender.
 *
 **/
extern const log4c_appender_type_t log4c_appender_type_stdout;

__LOG4C_END_DECLS

#endif

