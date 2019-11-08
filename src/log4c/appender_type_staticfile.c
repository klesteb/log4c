
/*
 * appender_type_staticfile.c
 *
 * Copyright 2019, Kevin L. Esteb (kevin@kesteb.us). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <log4c/appender.h>
#include <log4c/appender_type_staticfile.h>
#include <sd/malloc.h>
#include <sd/error.h>
#include <sd/domnode.h>
#include <sd/sd_xplatform.h>

/* Internal structs that defines the conf and the state info
 * for an instance of the appender_type_staticfile type.
 */    
struct __staticfile_conf {
  const char* sfc_logdir;
  const char* sfc_files_prefix; 
};

struct __staticfile_udata {
  struct __staticfile_conf sfu_conf;
  char *sfu_base_filename;
};

static char *staticfile_make_base_name(const char *, const char *);

/***************************************************************************
                Appender Interface functions: open, append, close, parser

****************************************************************************/
static int staticfile_open(log4c_appender_t *this) {

    int rc = 0; 
    staticfile_udata_t *sfup = log4c_appender_get_udata(this);  

    sd_debug("staticfile_appender_open[");

    /* xxx Make the init here more fine grained */
    
    if (sfup == NULL ){
     
        /* No info provided so set defaults */
        sd_debug("making new staticfile conf object, with default logdir/logprefix");
        sfup = staticfile_make_udata();

        staticfile_udata_set_logdir(sfup, STATICFILE_DEFAULT_LOG_DIR);
        staticfile_udata_set_files_prefix(sfup, STATICFILE_DEFAULT_LOG_PREFIX);
   
    } else {
        
        sd_debug("appender has udata already: logdir='%s' logprefix='%s'",
                 staticfile_udata_get_logdir(sfup),
                 staticfile_udata_get_files_prefix(sfup));
    }

    /* build the filename  */
    sfup->sfu_base_filename = staticfile_make_base_name(
        sfup->sfu_conf.sfc_logdir,
		sfup->sfu_conf.sfc_files_prefix);

    sd_debug("]");
      
    return rc;

}

/***************************************************************************/
static int staticfile_append(log4c_appender_t* this, 
			 const log4c_logging_event_t* a_event) {

    staticfile_udata_t* sfup = log4c_appender_get_udata(this); 
    int rc = 0;

    FILE *fp = NULL;
    
    sd_debug("staticfile_append[");
 
    if ((fp = fopen(sfup->sfu_base_filename, "a+")) != NULL) {
        
        rc = fprintf(fp, "%s\n", a_event->evt_rendered_msg);
        fclose(fp);
        
    }

    sd_debug("]");

    return rc;

}

/****************************************************************************/
static int staticfile_close(log4c_appender_t* this) {  
    
    int rc = 0;

    sd_debug("staticfile_close[");

    if (!this) {
        rc = -1; 
    }
 
    sd_debug("]");
    return rc;
    
}

/*******************************************************************************/
static int staticfile_parse(log4c_appender_t *this, void *a_node) {

    sd_domnode_t *dom = NULL;
    const char *logdir = ".";
    staticfile_udata_t *sfup = NULL;
    const char *logprefix = "unnamed.log";
    sd_domnode_t *anode = (sd_domnode_t *)a_node;

    dom = sd_domnode_attrs_get(anode, "logdir");
    if (dom && dom->value) {
        logdir = dom->value;
    }

    dom = sd_domnode_attrs_get(anode, "prefix");
    if (dom && dom->value) {
        logprefix = dom->value;
    }

    sd_debug("logdir='%s', prefix='%s'", logdir, logprefix);

    sfup = staticfile_make_udata();

    staticfile_udata_set_logdir(sfup, logdir);
    staticfile_udata_set_files_prefix(sfup, logprefix);

    log4c_appender_set_udata(this, sfup);
          
    return 0;
    
}

/*******************************************************************************/
staticfile_udata_t *staticfile_make_udata(void) {

    staticfile_udata_t *sfup = NULL;
    sfup = (staticfile_udata_t *)sd_calloc(1, sizeof(staticfile_udata_t));

    return sfup;

}

/*******************************************************************************/
LOG4C_API int staticfile_udata_set_logdir(staticfile_udata_t* sfup, const char *logdir){

    sfup->sfu_conf.sfc_logdir = strdup(logdir);  

    return(0);

}

/*******************************************************************************/
LOG4C_API const char * staticfile_udata_get_logdir(staticfile_udata_t* sfup){

    return(sfup->sfu_conf.sfc_logdir); 

}

/*******************************************************************************/
LOG4C_API int staticfile_udata_set_files_prefix( staticfile_udata_t* sfup, const char* fprefix){

    sfup->sfu_conf.sfc_files_prefix = strdup(fprefix);

    return 0;

}

/*******************************************************************************/
LOG4C_API const char *staticfile_udata_get_files_prefix( staticfile_udata_t* sfup){

    return(sfup->sfu_conf.sfc_files_prefix);

}

/*******************************************************************************/
static char *staticfile_make_base_name(const char *logdir, const char* prefix){

    int filename_len = 0;
    char* s = NULL;

    filename_len = strlen(logdir) + 1 +
        strlen(prefix) + 1 + 10; /* a margin */

    s = (char *)malloc(filename_len);      
    sprintf( s, "%s%s%s", logdir,
             FILE_SEP, prefix);

    return s;

}

/****************************************************************************/
const log4c_appender_type_t log4c_appender_type_staticfile = {
    "staticfile",
    staticfile_open,
    staticfile_append,
    staticfile_close,
    staticfile_parse
};

