/* $Id: appender_type_staticfile.h
 *
 * appender_type_staticfile.h
 * 
 * Copyright 2019, Kevin L. Esteb (kevin@kesteb.us). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef log4c_appender_type_staticfile_h
#define log4c_appender_type_staticfile_h

/**
 * @file appender_type_staticfile.h
 *
 * @brief Log4c static file appender interface.
 *
 * The static file appender implements a logging mechanism of
 * opening the file, write a log message and then closing the file.
 *
 **/

#include <log4c/defs.h>
#include <log4c/appender.h>

__LOG4C_BEGIN_DECLS

#define STATICFILE_DEFAULT_LOG_DIR "."
#define STATICFILE_DEFAULT_LOG_PREFIX "log"

typedef struct __staticfile_udata staticfile_udata_t; /* opaque */

/**
 * staticfile appender type definition.
 *
 * This should be used as a parameter to the log4c_appender_set_type()
 * routine to set the type of the appender.
 *
 **/
LOG4C_API const log4c_appender_type_t log4c_appender_type_staticfile;

/**
 * Get a new static file appender configuration object.
 * @return a new static file appender configuration object, otherwise NULL.
*/
LOG4C_API staticfile_udata_t* staticfile_make_udata(void);

/**
 * Set the logging directory in this static file appender configuration.
 * @param sfudatap the static file appender configuration object.
 * @param logdir the logging directory to set.
 * @return zero if successful, non-zero otherwise.
 */
LOG4C_API int staticfile_udata_set_logdir(
                staticfile_udata_t *sfudatap,
                const char* logdir);
/**
 * Set the prefix string in this rolling file appender configuration.
 * @param sfudatap the rolling file appender configuration object.
 * @param prefix the logging files prfix to use.
 * @return zero if successful, non-zero otherwise.
 */                            
LOG4C_API int staticfile_udata_set_files_prefix(
              staticfile_udata_t *sfudatap,
              const char * prefix);
/*
 * Get the logging directory in this static file appender configuration.
 * @param sfudatap the static file appender configuration object.
 * @return the logging directory.
 */                              
LOG4C_API const char* staticfile_udata_get_logdir(staticfile_udata_t* sfudatap);
              
/**
 * Get the prefix string in this static file appender configuration.
 * @param sfudatap the rolling file appender configuration object.
 * @return the prefix.
 */ 
LOG4C_API const char* staticfile_udata_get_files_prefix(
                            staticfile_udata_t* sfudatap);

__LOG4C_END_DECLS

#endif
