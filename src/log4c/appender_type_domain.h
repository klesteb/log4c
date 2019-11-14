/* $Id: appender_type_domain.h
 *
 * appender_type_omain.h
 * 
 * Copyright 2019, Kevin L. Esteb (kevin@kesteb.us). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef log4c_appender_type_domain_h
#define log4c_appender_type_domain_h

/**
 * @file appender_type_domain.h
 *
 * @brief Log4c domain socket appender interface.
 *
 * The domain socket appender implements a logging mechanism for
 * log messages to a domain socket.
 *
 **/

#include <log4c/defs.h>
#include <log4c/appender.h>

__LOG4C_BEGIN_DECLS

typedef struct __dmain_udata domain_udata_t; /* opaque */

/**
 * domain socket appender type definition.
 *
 * This should be used as a parameter to the log4c_appender_set_type()
 * routine to set the type of the appender.
 *
 **/
LOG4C_API const log4c_appender_type_t log4c_appender_type_domain;

/**
 * Get a new domain socket appender configuration object.
 * @return a new domain socket appender configuration object, otherwise NULL.
*/
LOG4C_API domain_udata_t* domain_make_udata(void);

/**
 * Set the path in this domain socket appender configuration.
 * @param dfudatap the domain socket appender configuration object.
 * @param path the path of the domain socket.
 * @return zero if successful, non-zero otherwise.
 */
LOG4C_API int domain_udata_set_path(
                domain_udata_t *dfudatap,
                const char* path);

/**
 * Get the path for this domain socket appender configuration.
 * @param dfudatap the static file appender configuration object.
 * @return the logging directory.
 **/                              
LOG4C_API const char* domain_udata_get_path(domain_udata_t *dfudatap);
              
__LOG4C_END_DECLS

#endif
